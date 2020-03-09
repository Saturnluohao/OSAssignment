#include "elevator2.h"

std::vector<clock_t> allWaitingTime;//记录所有请求所花费的时间，用于计算平均等待时间
int timePerFloor = 2000; //电梯在两层楼之间移动花费的时间
int delayPerFloor = 2000;//电梯停靠时所要花费的时间

bool compare(dest d1, dest d2)
{
	return d1.floor < d2.floor;
}

Elevator::Elevator(int _maxFloor)
{
	moveDirection = Direction::still;
	targetDirection = Direction::still;
	currentFloor = 1;
	targetFloor = 0;
	maxFloor = _maxFloor;
	moveStep = 0;
	targetStartTime = 0;
}

bool Elevator::addDestination(dest d)
{
	if (d.floor > maxFloor || d.floor < 1)
		return false;
	if ((moveDirection == Direction::down && !d.isRequest && currentFloor < d.floor) ||
		moveDirection == Direction::up && !d.isRequest && currentFloor > d.floor)
		return false;
	destQueue.push_back(d);
	destQueue.sort(compare);

	return true;
}

void Elevator::addRequest(int requestFloor, Direction direction)
{
	if (direction == Direction::still)
		return;
	dest d = dest(requestFloor);
	if (direction == Direction::up)
	{
		if (direction == moveDirection && requestFloor > currentFloor)
		{
			addDestination(requestFloor);
			if (targetDirection == Direction::down)
				resetTarget();
		}
		else
		{
			candidateUpDest.push_back(d);
			candidateUpDest.sort(compare);
		}
	}
	else
	{
		if (direction == moveDirection && requestFloor < currentFloor)
		{
			addDestination(requestFloor);
			if (targetDirection == Direction::up)
				resetTarget();
		}
		else
		{
			candidateDownDest.push_back(d);
			candidateDownDest.sort(compare);
		}
	}
}

double Elevator::getAverageWaitingTime()
{
	if (allWaitingTime.empty())
		return 0.0;
	double totalTime = 0.0;
	for (auto i : allWaitingTime)
	{
		totalTime += i;
	}
	return (totalTime / allWaitingTime.size()) / 1000;
}

//这里利用当前待停靠楼层队列以及候选停靠楼层队列预测当前请求可能需要等待的最少时间，
//根据方向的不同和相对位置分为多种情况，下面一一计算了相应的等待时间预测值
clock_t Elevator::getWaitingTime(int selfFloor, Direction direction)
{
	clock_t waitingTimePrediction = 0;
	std::list<dest>::iterator iter;
	if (!destQueue.empty())
	{
		for (iter = destQueue.begin(); iter != destQueue.end(); iter++)
		{
			waitingTimePrediction += delayPerFloor;
		}
	}
	if (moveDirection == Direction::still)
	{
		waitingTimePrediction += abs(currentFloor - selfFloor) * timePerFloor;
	}
	else if (moveDirection == direction)
	{
		if (direction == Direction::down)
		{
			if (selfFloor < currentFloor)
			{
				waitingTimePrediction += (currentFloor - selfFloor) * timePerFloor;
			}
			else
			{
				int finalFloor = destQueue.empty() ? targetFloor : destQueue.front().floor;
				waitingTimePrediction += (currentFloor - finalFloor) * timePerFloor;
				waitingTimePrediction += (selfFloor - finalFloor) * timePerFloor;
			}
		}
		else
		{
			if (selfFloor > currentFloor)
			{
				waitingTimePrediction += (selfFloor - currentFloor) * timePerFloor;
			}
			else
			{
				int finalFloor = destQueue.empty() ? targetFloor : destQueue.back().floor;
				waitingTimePrediction += (finalFloor - currentFloor) * timePerFloor;
				waitingTimePrediction += (finalFloor - selfFloor) * timePerFloor;
			}
		}
	}
	else
	{
		if (moveDirection == Direction::down)
		{
			int finalFloor = destQueue.empty() ? targetFloor : destQueue.front().floor;
			waitingTimePrediction += (currentFloor - finalFloor + abs(selfFloor - finalFloor))* timePerFloor;
		}
		else
		{
			int finalFloor = destQueue.empty() ? targetFloor : destQueue.back().floor;
			waitingTimePrediction += (finalFloor - currentFloor + abs(finalFloor - selfFloor)) * timePerFloor;
		}
	}
	return waitingTimePrediction;
}

void Elevator::resetTarget()
{
	targetFloor = 0;
	targetDirection = Direction::still;
}

void Elevator::schedule()
{
	if (moveDirection == Direction::still)//只在电梯静止时检查是否有候选请求
	{
		if (candidateDownDest.empty() && candidateUpDest.empty())//没有候选请求则不做什么
			return;
		else if (!candidateDownDest.empty())//处理向上的候选请求
		{
			int floor = candidateDownDest.back().floor;
			if (floor > currentFloor)//处理特殊情况
			{
				setMoveDirection(Direction::up);
				targetFloor = candidateDownDest.back().floor;
				targetDirection = Direction::down;
				targetStartTime = candidateDownDest.back().startTime;
			}
			else
			{
				copyDests(candidateDownDest);
				if(floor <= currentFloor)
					setMoveDirection(Direction::down);//改变方向
			}
		}
		else if (!candidateUpDest.empty())//处理向下的候选请求
		{
			int floor = candidateUpDest.front().floor;
			if (floor < currentFloor)//处理特殊情况
			{
				setMoveDirection(Direction::down);
				targetFloor = candidateUpDest.front().floor;
				targetDirection = Direction::up;
				targetStartTime = candidateUpDest.front().startTime;
			}
			else
			{
				copyDests(candidateUpDest);
				if (floor >= currentFloor)
					setMoveDirection(Direction::up);//改变方向
			}
		}
	}
}

void Elevator::updateMoveDir()
{
	if (!destQueue.empty())
	{
		if (currentFloor < destQueue.front().floor)
			setMoveDirection(Direction::up);
		if (currentFloor > destQueue.back().floor)
			setMoveDirection(Direction::down);
	}
}

void Elevator::setMoveDirection(Direction d)
{
	moveDirection = d;
	if (d == Direction::down)
		moveStep = -1;
	else if (d == Direction::up)
		moveStep = 1;
	else
		moveStep = 0;
}

void Elevator::copyDests(std::list<dest>& l)
{
	std::list<dest>::iterator iter = l.begin();
	for (; iter != l.end(); iter++)
	{
		addDestination(dest(iter->floor));
	}
	l.clear();
}

void Elevator::move()
{
	bool emitted = false;
	while (true)
	{
		msleep(4);//防止界面卡顿

		updateMoveDir();
		schedule();

		int moveDir = tranform();

		if (destQueue.empty() && targetFloor != 0)
		{
			if (currentFloor == targetFloor)
			{
				int tD = targetDirection == Direction::up ? 1 : -1;

				emit(destReached(currentFloor, tD, true));
				emitted = true;

				emit(startMove(tD, currentFloor));

				setMoveDirection(Direction::still);
				if (targetDirection == Direction::down)
					candidateDownDest.pop_back();
				else
					candidateUpDest.pop_front();
				resetTarget();
				allWaitingTime.push_back(clock() - targetStartTime);
				emit(updateWaitingTime(getAverageWaitingTime()));
				Sleep(timeSetting::DelayPerFloor());
				emit(startMove(0, currentFloor));
			}
			else
			{
				emit(startMove(moveDir, currentFloor));

				if (!emitted)
					emit(destReached(currentFloor, moveDir, false));
				emitted = false;

				emit(moveAnimation(moveStep));
				Sleep(timePerFloor);
				currentFloor += moveStep;
			}
			continue;
		}
		else if (destQueue.empty() || currentFloor > maxFloor || currentFloor < 1)
		{
			setMoveDirection(Direction::still);
			continue;
		}

		emit(startMove(moveDir, currentFloor));

		if (currentFloor == destQueue.front().floor)
		{
			dest d = destQueue.front();

			emit(destReached(currentFloor, moveDir, true));
			emitted = true;

			if (d.isRequest)
			{
				allWaitingTime.push_back(clock() - d.startTime);
				emit(updateWaitingTime(getAverageWaitingTime()));
			}
			destQueue.pop_front();
			Sleep(delayPerFloor);
		}
		else if (currentFloor == destQueue.back().floor)
		{
			dest d = destQueue.back();

			emit(destReached(currentFloor, moveDir, true));
			emitted = true;

			if (d.isRequest)
			{
				allWaitingTime.push_back(clock() - d.startTime);
				emit(updateWaitingTime(getAverageWaitingTime()));
			}
			destQueue.pop_back();
			Sleep(delayPerFloor);
		}
		
		if (!emitted)
			emit(destReached(currentFloor, moveDir, false));
		emitted = false;

		if (!destQueue.empty() && moveDir != 0)
		{
			emit(moveAnimation(moveDir));
			Sleep(timePerFloor);
			currentFloor += moveStep;
		}
		if(destQueue.empty())
			emit(startMove(0, currentFloor));
	}
}

int Elevator::tranform()
{
	if (moveDirection == Direction::up)
		return 1;
	else if (moveDirection == Direction::down)
		return -1;
	else
		return 0;
}

void Elevator::run()
{
	move();
}



elevatorGroupManager::elevatorGroupManager(int elevatorNum, int maxFloor)
{
	for (int i = 0; i < elevatorNum; i++)
		elevatorGroup.push_back(new Elevator(maxFloor));
}

elevatorGroupManager::~elevatorGroupManager()
{
	for (int i = 0; i < elevatorGroup.size(); i++)
	{
		delete elevatorGroup[i];
	}
}

void elevatorGroupManager::start()
{
	for (int i = 0; i < elevatorGroup.size(); i++) 
	{
		elevatorGroup[i]->start();
		std::cout << "elevator " << i << " started" << std::endl;
	}
}

void elevatorGroupManager::stop()
{
	for (int i = 0; i < elevatorGroup.size(); i++)
	{
		elevatorGroup[i]->exit();
	}
}

void elevatorGroupManager::addRequest(int selfFloor, Direction direction)
{
	if (direction == Direction::still)
		return;
	int bestIndex = 0;
	clock_t minWaitingTime = -1;
	for (int i = 0; i < elevatorGroup.size(); i++) //找到当前电梯群中可以使该请求等待时间最小的
	{
		clock_t waitingTime = elevatorGroup[i]->getWaitingTime(selfFloor, direction);
		if (i == 0)
			minWaitingTime = waitingTime;
		if (waitingTime < minWaitingTime)
		{
			minWaitingTime = waitingTime;
			bestIndex = i;
		}
	}
	elevatorGroup[bestIndex]->addRequest(selfFloor, direction);
	emit(requestHandled(bestIndex, selfFloor));
}


int timeSetting::TimePerFloor()
{
	return timePerFloor;
}

void timeSetting::setTimePerFloor(int _timePerFloor)
{
	if (_timePerFloor <= 0)
		return;
	timePerFloor = _timePerFloor * 1000;
}

int timeSetting::DelayPerFloor()
{
	return delayPerFloor;
}

void timeSetting::setDelayPerFloor(int _delayPerFloor)
{
	if (_delayPerFloor <= 0)
		return;
	delayPerFloor = _delayPerFloor * 1000;
}