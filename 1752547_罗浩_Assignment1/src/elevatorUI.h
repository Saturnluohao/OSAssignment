#pragma once

#include <QtWidgets/QMainWindow>
#include <QObject>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QThread>
#include <QPropertyAnimation>
#include <QPolygon>
#include <QPushButton>
#include <QString>
#include <QLCDNumber>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include<QScrollArea>
#include "elevator2.h"


class elevatorThread : public QThread
{
	Q_OBJECT
public:
	elevatorThread();
	elevatorThread(int elevatorNum, int maxFloor);
	~elevatorThread();
protected:
	void run();
private:
	elevatorGroupManager* manager;
	friend class elevatorUI;
};


class floorsItem : public QWidget
{
	Q_OBJECT

public:
	floorsItem(int _elevatorNum, int _maxFloor = 20, QWidget* parent = 0) : elevatorNum(_elevatorNum), maxFloor(_maxFloor), QWidget(parent){}

private:
	int maxFloor;
	int elevatorNum;

	void paintEvent(QPaintEvent* event) override;

};





class directionButton : public QWidget
{
	Q_OBJECT

public:
	directionButton(QString direciton, QWidget* parent = Q_NULLPTR, int width = 20, int height = 20);

public:
	int direction;
	int width;
	int height;
	QBrush brush;
	bool isResponsed;
	void response();
	void deResponse();

signals:
	void dirButtonPressed(int dir);
protected:
	QRectF boundingRect() const;
	void paintEvent(QPaintEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
};






class arrow : public QWidget
{
	Q_OBJECT
public:
	arrow(QString dir = "still", QWidget* parent = Q_NULLPTR, int _width = 20, int _height = 30);

	int direction, width, height;
	void setDirection(int tag);
private:

	virtual QRectF boundingRect()const;
	virtual void paintEvent(QPaintEvent* event);
};



class outerDisplayer : public QWidget
{
	Q_OBJECT
public:
	outerDisplayer(int _floor, QWidget* parent = Q_NULLPTR);

	void setDisplayerState(int dir, int _floorNum);

	int floor;
	arrow* moveArrow;
private:
	QLCDNumber* floorNum;
};




class outerDisplayerGroup : public QWidget
{
	Q_OBJECT
public:
	outerDisplayerGroup(int gap, int displayerNum, QWidget* parent = Q_NULLPTR);
	void onStateChanged(int moveDir, int floor);
	void onDestReached(int _floor, int moveDir, bool isReq);

	void timerEvent(QTimerEvent * event);
	void stopTwinkle();
private:
	int timerId;
	arrow* willTwinkle;
};





class outerButton : public QWidget
{
	Q_OBJECT
public:
	outerButton(QWidget* parent = Q_NULLPTR, int speciality = 0);

	directionButton* upButton = NULL;
	directionButton* downButton = NULL;
};





class outterButtonGroup : public QWidget
{
	Q_OBJECT
public:
	outterButtonGroup(int _floor, int gap, int buttonNum, QWidget* parent=Q_NULLPTR, int speciality = 0);

	int floor;

	void onRequestHandled(int _floor, int moveDir, bool isRequest);
signals:
	void request(int floor, Direction dir);

private:
	void lighten(int tag);
	void darken(int dir);
};





class innerButton : public QPushButton
{
	Q_OBJECT
public:
	innerButton(int floorIndex, QWidget* parent = Q_NULLPTR);

	virtual void mousePressEvent(QMouseEvent* event);

signals:
	void destFloor(int floorIndex);
private:
	int index;
	bool isPressed;

	void lightenButton();
	void darkenButton();

	friend class innerControlPanel;
};






class doorControlButton : public QPushButton
{
	Q_OBJECT
public:
	doorControlButton(int _flag, QWidget* parent = Q_NULLPTR, int _width = 60, int _height = 20);

	void mousePressEvent(QMouseEvent* event);
protected:
	int flag;
	int width;
	int height;
	void paintEvent(QPaintEvent* event);
};






class innerControlPanel : public QWidget
{
	Q_OBJECT
public:
	innerControlPanel(int elevatorIndex, int floorNum = 20, QWidget* parent = Q_NULLPTR);
	void addRequest(int floor);
	void onDestChanged(int floor, int moveDir, bool isRequest);
	void onAlert();
	void onUpdateDir(int dir, int currentFloor);

	int currentFloor, dir;

signals:
	void request(dest floor);
private:

	QGridLayout* gridLayout;
	QVector<innerButton*> buttons;
	QPushButton* alertButton;
	doorControlButton* openDoor;
	doorControlButton* closeDoor;
};






class elevatorItem : public QWidget
{
	Q_OBJECT

public:
	elevatorItem(int _elevatorId, int floorNum = 20, QWidget* parent = Q_NULLPTR);
	void startTwinkle();
	void stopTwinkle();

	void onDestReached(int floor, int moveDir, bool isRequest);
	void onAnimation(int moveStep);

	innerControlPanel* icp;

private:
	int maxFloor;
	bool isLight;
	int index;
	int timerId;
	QBrush brush;
	QPropertyAnimation* annimation;

	void startAnimation(int moveStep);
	void stopAnimaiton();

	virtual void mousePressEvent(QMouseEvent* event);

	virtual void paintEvent(QPaintEvent* event);
	virtual void timerEvent(QTimerEvent* event);
};










class elevatorUI : public QMainWindow
{
	Q_OBJECT

public:
	elevatorUI(int elevatorNum = 5, int maxFloor = 20, QWidget *parent = Q_NULLPTR);

	void onHandled(int elevatorIndex, int floor);
	void onUpdateWaitingTIme(double averageWaitingTime);

private:
	elevatorThread* eThread;
	floorsItem* frame;
	QVector<elevatorItem*> elevatorItems;
	QVector<outerDisplayerGroup*> outerDisplayerGroups;
	QVector<outterButtonGroup*>outterButtonGroups;
	QLCDNumber* waitingTimeDisplayer;
	QLabel* timeInfo;
	timeSetting* timeSetter;
};


class guider : public QWidget
{
	Q_OBJECT
public:
	guider();

	void createElevatorUI();
	void setFloorNum(int num);
	void setElevatorNum(int num);

private:
	int floorNum;
	int elevatorNum;
};