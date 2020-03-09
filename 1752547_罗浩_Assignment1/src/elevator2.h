#pragma once
#include<queue>
#include<list>
#include<time.h>
#include<Windows.h>
#include<iostream>
#include<vector>
#include<future>
#include<QObject>
#include <QThread>

//表示方向的枚举类
enum Direction { still, up, down };

//记录电梯将要停靠的楼层及相关信息
struct dest
{
	int floor;//楼层
	bool isRequest;//电梯内或电梯外发送请求
	clock_t startTime;//若是电梯外的请求则表示开始请求时刻
	dest(int _floor, bool _isRequest = true) : floor(_floor), isRequest(_isRequest) , startTime(clock()){}
};


//电梯类
//这里特殊情况指电梯响应用户的请求后将要运动的方向和用户请求的方向相反
class Elevator : public QThread
{
	Q_OBJECT

public:
	Elevator(int _maxFloor);
	Elevator() { maxFloor = 20; }

	void addRequest(int requestFloor, Direction direction);//添加请求（楼层外）
	bool addDestination(dest d);//添加将停靠的楼层（电梯内）
private:
	Direction moveDirection;//电梯现在的移动方向
	Direction targetDirection;//特殊情况用户请求的方向
	int currentFloor;//电梯现在所在的楼层
	int targetFloor;//特殊情况用户所在的楼层
	int maxFloor;//电梯最高楼层数
	int moveStep;//电梯移动步伐
	clock_t targetStartTime;//特殊情况用户请求开始的时间
	std::list<dest> destQueue;//电梯现在将要停靠楼层队列
	std::list<dest> candidateUpDest;//电梯从向下运动转为向上运动时将要停靠的楼层队列
	std::list<dest> candidateDownDest; //电梯从向上运动转到向下运动时将要停靠的楼层队列

	void schedule();//调度，根据candidateUpDest和candidateDownDest更新destQueue
	void updateMoveDir();//
	void setMoveDirection(Direction d);//设置运动方向和运动步伐
	void copyDests(std::list<dest>& l);//将候选停靠队列复制到destQueue
	double getAverageWaitingTime();//计算目前为止所有请求的平均等待时间
	clock_t getWaitingTime(int selfFloor, Direction direction);//预测某个请求需要等待的时间
	void resetTarget();//重置
	void move();//控制电梯运动的函数
	int tranform();//将方向枚举类转化为int型

	friend class elevatorGroupManager;
public:
signals:
	void destReached(int floor, int moveDir, bool isRequest);//停靠楼层时的信号
signals:
	void startMove(int moveDir, int currentFloor);//开始移动时的信号
signals:
	void updateWaitingTime(double averageWaitingTime);//更新平均等待时间的信号
signals:
	void moveAnimation(int moveStep);

protected:
	void run();//重写线程启动函数
};


//电梯管理类
class elevatorGroupManager : public QThread
{
	Q_OBJECT
public:
	elevatorGroupManager(){}
	elevatorGroupManager(int elevatorNum, int maxFloor);
	~elevatorGroupManager();
	void start();//启动各电梯线程
	void stop();//停止各电梯线程
	void addRequest(int selfFloor, Direction direction);//电梯组管理者将用户请求分配给最合适电梯

signals:
	void requestHandled(int elevatorIndex, int floor);//用户请求被电梯响应完成后的信号
private:
	std::vector<Elevator*> elevatorGroup;//电梯组
	friend class elevatorUI;
};

//相关时间设置类
class timeSetting : public QObject
{
	Q_OBJECT
public:
	static int TimePerFloor();
	void setTimePerFloor(int _timePerFloor);//设置电梯在两层楼之间移动花费的时间
	static int DelayPerFloor();
	void setDelayPerFloor(int _delayPerFloor);//设置电梯停靠时所要花费的时间
};
