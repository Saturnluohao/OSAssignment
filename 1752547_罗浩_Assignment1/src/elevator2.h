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

//��ʾ�����ö����
enum Direction { still, up, down };

//��¼���ݽ�Ҫͣ����¥�㼰�����Ϣ
struct dest
{
	int floor;//¥��
	bool isRequest;//�����ڻ�����ⷢ������
	clock_t startTime;//���ǵ�������������ʾ��ʼ����ʱ��
	dest(int _floor, bool _isRequest = true) : floor(_floor), isRequest(_isRequest) , startTime(clock()){}
};


//������
//�����������ָ������Ӧ�û��������Ҫ�˶��ķ�����û�����ķ����෴
class Elevator : public QThread
{
	Q_OBJECT

public:
	Elevator(int _maxFloor);
	Elevator() { maxFloor = 20; }

	void addRequest(int requestFloor, Direction direction);//�������¥���⣩
	bool addDestination(dest d);//��ӽ�ͣ����¥�㣨�����ڣ�
private:
	Direction moveDirection;//�������ڵ��ƶ�����
	Direction targetDirection;//��������û�����ķ���
	int currentFloor;//�����������ڵ�¥��
	int targetFloor;//��������û����ڵ�¥��
	int maxFloor;//�������¥����
	int moveStep;//�����ƶ�����
	clock_t targetStartTime;//��������û�����ʼ��ʱ��
	std::list<dest> destQueue;//�������ڽ�Ҫͣ��¥�����
	std::list<dest> candidateUpDest;//���ݴ������˶�תΪ�����˶�ʱ��Ҫͣ����¥�����
	std::list<dest> candidateDownDest; //���ݴ������˶�ת�������˶�ʱ��Ҫͣ����¥�����

	void schedule();//���ȣ�����candidateUpDest��candidateDownDest����destQueue
	void updateMoveDir();//
	void setMoveDirection(Direction d);//�����˶�������˶�����
	void copyDests(std::list<dest>& l);//����ѡͣ�����и��Ƶ�destQueue
	double getAverageWaitingTime();//����ĿǰΪֹ���������ƽ���ȴ�ʱ��
	clock_t getWaitingTime(int selfFloor, Direction direction);//Ԥ��ĳ��������Ҫ�ȴ���ʱ��
	void resetTarget();//����
	void move();//���Ƶ����˶��ĺ���
	int tranform();//������ö����ת��Ϊint��

	friend class elevatorGroupManager;
public:
signals:
	void destReached(int floor, int moveDir, bool isRequest);//ͣ��¥��ʱ���ź�
signals:
	void startMove(int moveDir, int currentFloor);//��ʼ�ƶ�ʱ���ź�
signals:
	void updateWaitingTime(double averageWaitingTime);//����ƽ���ȴ�ʱ����ź�
signals:
	void moveAnimation(int moveStep);

protected:
	void run();//��д�߳���������
};


//���ݹ�����
class elevatorGroupManager : public QThread
{
	Q_OBJECT
public:
	elevatorGroupManager(){}
	elevatorGroupManager(int elevatorNum, int maxFloor);
	~elevatorGroupManager();
	void start();//�����������߳�
	void stop();//ֹͣ�������߳�
	void addRequest(int selfFloor, Direction direction);//����������߽��û�������������ʵ���

signals:
	void requestHandled(int elevatorIndex, int floor);//�û����󱻵�����Ӧ��ɺ���ź�
private:
	std::vector<Elevator*> elevatorGroup;//������
	friend class elevatorUI;
};

//���ʱ��������
class timeSetting : public QObject
{
	Q_OBJECT
public:
	static int TimePerFloor();
	void setTimePerFloor(int _timePerFloor);//���õ���������¥֮���ƶ����ѵ�ʱ��
	static int DelayPerFloor();
	void setDelayPerFloor(int _delayPerFloor);//���õ���ͣ��ʱ��Ҫ���ѵ�ʱ��
};
