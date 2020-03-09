#include "elevatorUI.h"

elevatorThread::elevatorThread()
{
	manager = new elevatorGroupManager(5, 20);
}

elevatorThread::elevatorThread(int elevatorNum, int maxFloor)
{
	manager = new elevatorGroupManager(elevatorNum, maxFloor);
}

void elevatorThread::run()
{
	manager->start();
}

elevatorThread::~elevatorThread()
{
	manager->stop();
	delete manager;
}





void floorsItem::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);
	
	QPainter painter(this);

	int wholeWidth = 140 * elevatorNum + 80;

	painter.drawRect(0, 0, wholeWidth, 40 * maxFloor);
	for (int i = 1; i <= maxFloor; i++)
	{
		painter.drawLine(0, 40 * i, wholeWidth, 40 * i);
	}
}




directionButton::directionButton(QString _direction, QWidget * parent, int _width, int _height) : QWidget(parent)
{
	if (_direction == "up")
		direction = 1;
	else
		direction = -1;
	isResponsed = false;
	brush = QBrush(Qt::white);
	height = _height;
	width = _width;
}

void directionButton::response()
{
	brush.setColor(Qt::red);
	update();
	isResponsed = true;
}

void directionButton::deResponse()
{
	brush.setColor(Qt::white);
	update();
	isResponsed = false;
}

QRectF directionButton::boundingRect() const
{
	return QRectF(0, 0, width, height);
}

void directionButton::paintEvent(QPaintEvent* event)
{
	QVector<QPoint> points;
	if (direction == -1)
		points = { QPoint(0, 0), QPoint(width, 0), QPoint(width / 2, height) };
	else
		points = { QPoint(0, height), QPoint(width, height), QPoint(width / 2, 0) };
	auto triangle = QPolygon(points);
	QPainter painter(this);
	painter.setBrush(brush);
	painter.drawPolygon(triangle);
}

void directionButton::mousePressEvent(QMouseEvent * event)
{
	if (!isResponsed)
		emit(dirButtonPressed(direction));
}




arrow::arrow(QString dir, QWidget* parent, int _width, int _height) : QWidget(parent)
{
	if (dir == "up")
		direction = 1;
	else if (dir == "down")
		direction = -1;
	else
		direction = 0;
	width = _width;
	height = _height;
}

void arrow::setDirection(int tag)
{
	direction = tag;
	update();
}

QRectF arrow::boundingRect()const
{
	return QRectF(0, 0, width, height);
}

void arrow::paintEvent(QPaintEvent* event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setPen(QPen(Qt::white, 3));

	if (direction != 0)
		painter.setPen(QPen(Qt::red, 3));
	painter.drawLine(width / 2, 0, width / 2, height);

	painter.setPen(QPen(Qt::white, 3));

	if (direction == 1)
		painter.setPen(QPen(Qt::red, 3));
	painter.drawLine(width / 2, 0, 0, width / 4);
	painter.drawLine(width / 2, 0, width, width / 4);

	painter.setPen(QPen(Qt::white, 3));

	if (direction == -1)
		painter.setPen(QPen(Qt::red, 3));
	painter.drawLine(width / 2, height, 0, height * 3 / 4);
	painter.drawLine(width / 2, height, width, height * 3 / 4);
}




outerDisplayer::outerDisplayer(int _floor, QWidget* parent) : QWidget(parent)
{
	floor = _floor;
	floorNum = new QLCDNumber(2, this);
	moveArrow = new arrow("still", this);
	floorNum->display(1);
	floorNum->setStyleSheet("color:red");
	moveArrow->setProperty("geometry", QRect(0, 0, moveArrow->width, moveArrow->height));
	floorNum->setProperty("geometry", QRect(moveArrow->width, 0, 30, 30));
}

void outerDisplayer::setDisplayerState(int dir, int _floorNum)
{
	floorNum->display(_floorNum);
	moveArrow->setDirection(dir);
}






outerDisplayerGroup::outerDisplayerGroup(int gap, int playerNum, QWidget* parent) : QWidget(parent)
{
	for (int i = 0; i < playerNum; i++)
	{
		auto displayer = new outerDisplayer(playerNum - i, this);
		displayer->setProperty("geometry", QRect(0, gap * i, 50, 30));
	}
}

void outerDisplayerGroup::onStateChanged(int moveDir, int floor)
{
	auto displayerList = this->children();
	foreach(QObject* obj, displayerList)
	{
		outerDisplayer* button = qobject_cast<outerDisplayer*>(obj);
		button->setDisplayerState(moveDir, floor);
	}
}

void outerDisplayerGroup::onDestReached(int _floor, int moveDir, bool isReq)
{
	if (!isReq || this->children().empty())
		return;
	foreach(QObject* obj, this->children())
	{
		auto displayer = qobject_cast<outerDisplayer*>(obj);
		if (displayer && displayer->floor == _floor)
			willTwinkle = displayer->moveArrow;
	}
	timerId = startTimer(500);
	QTimer::singleShot(timeSetting::DelayPerFloor(), this, &outerDisplayerGroup::stopTwinkle);
}

void outerDisplayerGroup::stopTwinkle()
{
	killTimer(timerId);
	willTwinkle->setHidden(false);
}

void outerDisplayerGroup::timerEvent(QTimerEvent * event)
{
	if (willTwinkle->isHidden())
		willTwinkle->setHidden(false);
	else
		willTwinkle->setHidden(true);
}






outerButton::outerButton(QWidget* parent, int speciality) : QWidget(parent)
{
	if(speciality == 0 || speciality == 1)
		upButton = new directionButton("up", this);
	if (speciality == 0 || speciality == 2)
	{
		downButton = new directionButton("down", this);
		downButton->setProperty("geometry", QRect(0, 20, 20, 20));
	}
	this->resize(20, 40);
}




outterButtonGroup::outterButtonGroup(int _floor, int gap, int buttonNum, QWidget* parent, int speciality) : QWidget(parent)
{
	for (int i = 0; i < buttonNum; i++)
	{
		auto button = new outerButton(this, speciality);
		if(speciality == 0 || speciality == 1)
			connect(button->upButton, &directionButton::dirButtonPressed, this, &outterButtonGroup::lighten);
		if (speciality == 0 || speciality == 2)
			connect(button->downButton, &directionButton::dirButtonPressed, this, &outterButtonGroup::lighten);
		button->setProperty("geometry", QRect(gap * i, 0, 20, 40));
	}
	floor = _floor;
}

void outterButtonGroup::onRequestHandled(int _floor, int moveDir, bool isRequest)
{
	if (isRequest && floor == _floor)
		darken(moveDir);
}

void outterButtonGroup::lighten(int tag)
{
	auto buttonList = this->children();
	Direction dir = Direction::down;
	foreach(QObject* obj, buttonList)
	{
		outerButton* button = qobject_cast<outerButton*>(obj);
		if (tag == 1)
		{
			button->upButton->response();
			dir = Direction::up;
		}
		else
			button->downButton->response();
	}
	emit(request(floor, dir));
}

void outterButtonGroup::darken(int dir)
{
	auto buttonList = this->children();
	foreach(QObject* obj, buttonList)
	{
		outerButton* button = qobject_cast<outerButton*>(obj);
		if (button->upButton && dir == 1)
			button->upButton->deResponse();
		if(button->downButton && dir == -1)
			button->downButton->deResponse();
	}
}





innerButton::innerButton(int floorIndex, QWidget* parent) : QPushButton(parent)
{
	index = floorIndex;
	this->setText(QString::number(floorIndex));
	this->setProperty("geometry", QRect(0, 0, 20, 20));
	this->setStyleSheet("background-color:white");
	isPressed = false;
}

void innerButton::mousePressEvent(QMouseEvent * event)
{
	if (!isPressed)
	{
		auto parent = qobject_cast<innerControlPanel*>(this->parent());
		if (parent->dir == 0 || parent->dir * (parent->currentFloor - index) < 0)
		{
			emit(destFloor(index));
			lightenButton();
		}
	}
}

void innerButton::lightenButton()
{
	this->setStyleSheet("background-color:orange");
	isPressed = true;
	update();
}

void innerButton::darkenButton()
{
	this->setStyleSheet("background-color:white");
	isPressed = false;
	update();
}






doorControlButton::doorControlButton(int _flag, QWidget * parent, int _width, int _height) : QPushButton(parent)
{
	flag = _flag;
	width = _width;
	height = _height;
}

void doorControlButton::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	if (flag == 0)
	{
		painter.drawLine(0, height / 2, width / 2 - 2, 0);
		painter.drawLine(0, height / 2, width / 2 - 2, height);
		painter.drawLine(width, height / 2, width / 2 + 2, 0);
		painter.drawLine(width, height / 2, width / 2 + 2, height);
	}
	else
	{
		painter.drawLine(0, 0, width / 2 - 2, height / 2);
		painter.drawLine(0, height, width / 2 - 2, height / 2);
		painter.drawLine(width, 0, width / 2 + 2, height / 2);
		painter.drawLine(width, height, width / 2 + 2, height / 2);
	}
}

void doorControlButton::mousePressEvent(QMouseEvent * event)
{
	if (flag == 0)
	{

	}
	else
	{

	}
}








innerControlPanel::innerControlPanel(int elevatorIndex, int floorNum, QWidget* parent) : QWidget(parent)
{
	int col = 2;
	int row = floorNum % col == 0 ? floorNum / col : floorNum / col+ 1;
	gridLayout = new QGridLayout;
	auto elevatorInfo = new QLabel("     elevator " + QString::number(elevatorIndex));
	elevatorInfo->setStyleSheet("color:green");
	elevatorInfo->setFont(QFont("Microsoft YaHei", 15, 50));
	gridLayout->addWidget(elevatorInfo, 0, 0, 1, 2);

	if (buttons.empty())
	{
		for (int i = 0; i < floorNum; i++)
		{
			buttons.push_back(new innerButton(i + 1));
			connect(buttons[i], &innerButton::destFloor, this, &innerControlPanel::addRequest);
		}
	}

	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row && i * row + j < floorNum ; j++)
		{
			gridLayout->addWidget(buttons[i * row + j], j + 1, i, 1, 1);
		}
	}
	alertButton = new QPushButton("ALERT");
	alertButton->setStyleSheet("background-color: red");
	openDoor = new doorControlButton(0);
	closeDoor = new doorControlButton(1);

	gridLayout->addWidget(openDoor, row + 1, 0, 1, 1);
	gridLayout->addWidget(closeDoor, row + 1, 1, 1, 1);
	gridLayout->addWidget(alertButton, row+2, 0, 1, 2);

	connect(alertButton, &QPushButton::clicked, this, &innerControlPanel::onAlert);


	this->setLayout(gridLayout);
}

void innerControlPanel::addRequest(int floor)
{
	emit(request(dest(floor, false)));
}

void innerControlPanel::onDestChanged(int floor, int moveDir, bool isRequest)
{
	auto buttonList = this->children();
	dir = moveDir;
	currentFloor = floor;
	foreach(QObject* obj, buttonList)
	{
		innerButton* button = qobject_cast<innerButton*>(obj);
		if (button != NULL && button->index == floor)
		{
			button->darkenButton();
			break;
		}
	}
}

void innerControlPanel::onAlert()
{
	auto alertWindow = new QWidget;
	auto message = new QLabel("            We have received ALERT message\nPlease stay calm and we will handle it at once!", alertWindow);
	message->setFont(QFont("Microsoft YaHei", 14, 50));
	message->setStyleSheet("color:red");
	alertWindow->resize(450, 100);
	alertWindow->setWindowTitle("Alert Window");
	alertWindow->show();
}

void innerControlPanel::onUpdateDir(int _dir, int currentFloor)
{
	dir = _dir;
}






elevatorItem::elevatorItem(int elevatorId, int floorNum, QWidget* parent) : QWidget(parent)
{
	isLight = true;
	maxFloor = floorNum;
	this->setStyleSheet("background-color:red");
	icp = new innerControlPanel(elevatorId + 1, floorNum);
	brush = QBrush(Qt::blue);
	annimation = new QPropertyAnimation(this, "geometry", this);
	index = 1;
}

void elevatorItem::startTwinkle()
{
	timerId = startTimer(500);
}

void elevatorItem::stopTwinkle()
{
	killTimer(timerId);
	brush = QBrush(Qt::blue);
	update();
	isLight = true;
}


void elevatorItem::onDestReached(int floor,int moveDir, bool isRequest)
{
	/*if (isRequest)
	{
		startTwinkle();
		QTimer::singleShot(timeSetting::DelayPerFloor(), this, &elevatorItem::stopTwinkle);
	}*/
	if (index != floor)
	{
		stopAnimaiton();
		auto op = this->geometry();
		this->setProperty("geometry", QRect(op.x(), 40*(maxFloor - floor), op.width(), op.height()));
	}
	index = floor;
}

void elevatorItem::onAnimation(int moveStep)
{
	startAnimation(moveStep);
	QTimer::singleShot(timeSetting::DelayPerFloor(), this, &elevatorItem::stopAnimaiton);
}

void elevatorItem::startAnimation(int moveStep)
{
	auto op = this->geometry();
	annimation->setStartValue(op);
	annimation->setEndValue(QRect(op.x(), op.y() - 40 * moveStep, op.width(), op.height()));
	annimation->setDuration(timeSetting::TimePerFloor());
	annimation->start();
}

void elevatorItem::stopAnimaiton()
{
	annimation->stop();
}

void elevatorItem::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);

	QPainter painter(this);
	painter.setBrush(brush);
	painter.drawRect(0, 0, 30, 40);
}

void elevatorItem::timerEvent(QTimerEvent * event)
{
	if (isLight)
	{
		brush = QBrush(QColor(0, 0, 0, 0));
		update();
		isLight = false;
	}
	else
	{
		brush = QBrush(Qt::blue);
		update();
		isLight = true;
	}
}

void elevatorItem::mousePressEvent(QMouseEvent * event)
{
	Q_UNUSED(event);

	icp->show();
}










elevatorUI::elevatorUI(int elevatorNum, int maxFloor, QWidget *parent)
	: QMainWindow(parent)
{
	eThread = new elevatorThread(elevatorNum, maxFloor);
	frame = new floorsItem(elevatorNum, maxFloor, this);
	frame->resize(elevatorNum * 140 + 130, maxFloor * 40 + 40);
	waitingTimeDisplayer = new QLCDNumber(10, this);
	waitingTimeDisplayer->setSegmentStyle(QLCDNumber::Flat);
	waitingTimeDisplayer->setStyleSheet("color:green");
	waitingTimeDisplayer->setProperty("geometry", QRect(150 + 140 * elevatorNum, 100, 100, 40));
	timeInfo = new QLabel("Average waiting time/s",this);
	timeInfo->setFont(QFont("Microsoft YaHei", 12, 50));
	timeInfo->setStyleSheet("color::purple");
	timeInfo->setAlignment(Qt::AlignLeft);
	timeInfo->setProperty("geometry", QRect(120 + 140 * elevatorNum, 55, 200, 40));

	timeSetter = new timeSetting;

	auto vLayout = new QVBoxLayout(this);
	auto timePerFloorSetting = new QSlider(Qt::Horizontal, this);
	auto delayPerFloorSetting = new QSlider(Qt::Horizontal, this);
	timePerFloorSetting->setRange(0, 10);
	delayPerFloorSetting->setRange(0, 10);
	timePerFloorSetting->setSingleStep(1);
	timePerFloorSetting->setTickPosition(QSlider::TicksAbove);
	timePerFloorSetting->setValue(2);
	delayPerFloorSetting->setSingleStep(1);
	delayPerFloorSetting->setTickPosition(QSlider::TicksAbove);
	delayPerFloorSetting->setValue(2);
	

	auto timePerFloorInfo = new QLabel("Cost time between two floors (Drag the cursor to set)", this);
	auto delayPerFloorInfo = new QLabel("Delay time per floor (Drag the cursor to set)", this);
	timePerFloorInfo->setFont(QFont("Microsoft YaHei", 10, 50));
	timePerFloorInfo->setStyleSheet("color:green");
	delayPerFloorInfo->setFont(QFont("Microsoft YaHei", 10, 50));
	delayPerFloorInfo->setStyleSheet("color:green");

	vLayout->addWidget(timeInfo);
	vLayout->addWidget(waitingTimeDisplayer);
	vLayout->addSpacing(50);
	vLayout->addWidget(timePerFloorInfo);
	vLayout->addWidget(timePerFloorSetting);
	vLayout->addWidget(delayPerFloorInfo);
	vLayout->addWidget(delayPerFloorSetting);
	vLayout->setGeometry(QRect(90 + 140 * elevatorNum, 20 * maxFloor - 150, 330, 200));

	connect(timePerFloorSetting, &QSlider::valueChanged, timeSetter, &timeSetting::setTimePerFloor);
	connect(delayPerFloorSetting, static_cast<void(QSlider::*)(int)>(&QSlider::valueChanged), timeSetter, &timeSetting::setDelayPerFloor);

	for (int i = 0; i < maxFloor; i++)
	{
		int speciality = 0;
		if (i == 0)
			speciality = 1;
		if (i == maxFloor-1)
			speciality = 2;
		outterButtonGroups.push_back(new outterButtonGroup(i+1, 140, (elevatorNum  > 1 ? elevatorNum - 1 : 1), this, speciality));
		outterButtonGroups[i]->setProperty("geometry", QRect(180, 40 * (maxFloor-1-i), 120*elevatorNum, 40));
		connect(outterButtonGroups[i], &outterButtonGroup::request, eThread->manager, &elevatorGroupManager::addRequest);

		auto floorInfo = new QLabel("Floor " + QString::number(maxFloor - i), this);
		floorInfo->setFont(QFont("Microsoft YaHei", 12, 50));
		floorInfo->setStyleSheet("color:green");
		floorInfo->setGeometry(QRect(0, i * 40 + 5, 70, 20));
	}
	for (int i = 0; i < elevatorNum; i++)
	{
		outerDisplayerGroups.push_back(new outerDisplayerGroup(40, maxFloor, this));
		elevatorItems.push_back(new elevatorItem(i, maxFloor, this));
		elevatorItems[i]->setProperty("geometry", QRect(80 + 140 * i, 40 * (maxFloor-1), 30, 40));
		outerDisplayerGroups[i]->setProperty("geometry", QRect(110 + 140 * i, 0, 50, 40 * maxFloor));

		auto elevatorIndex = new QLabel("elevator " + QString::number(i+1), this);
		elevatorIndex->setFont(QFont("Microsoft YaHei", 12, 50));
		elevatorIndex->setStyleSheet("color:green");
		elevatorIndex->setGeometry(QRect(i * 140 + 60, maxFloor * 40 +10, 80, 20));

		connect(eThread->manager->elevatorGroup[i], &Elevator::destReached, elevatorItems[i], &elevatorItem::onDestReached);
		connect(elevatorItems[i]->icp, &innerControlPanel::request, eThread->manager->elevatorGroup[i], &Elevator::addDestination);
		connect(eThread->manager->elevatorGroup[i], &Elevator::destReached, elevatorItems[i]->icp, &innerControlPanel::onDestChanged);
		connect(eThread->manager->elevatorGroup[i], &Elevator::startMove, outerDisplayerGroups[i], &outerDisplayerGroup::onStateChanged);
		connect(eThread->manager, &elevatorGroupManager::requestHandled, this, &elevatorUI::onHandled);
		connect(eThread->manager->elevatorGroup[i], &Elevator::updateWaitingTime, this, &elevatorUI::onUpdateWaitingTIme);
		connect(eThread->manager->elevatorGroup[i], &Elevator::destReached, outerDisplayerGroups[i], &outerDisplayerGroup::onDestReached);
		connect(eThread->manager->elevatorGroup[i], &Elevator::moveAnimation, elevatorItems[i], &elevatorItem::onAnimation);
		connect(eThread->manager->elevatorGroup[i], &Elevator::startMove, elevatorItems[i]->icp, &innerControlPanel::onUpdateDir);
	}


	eThread->start();
}

void elevatorUI::onHandled(int elevatorIndex, int floor)
{
	connect(eThread->manager->elevatorGroup[elevatorIndex], &Elevator::destReached, outterButtonGroups[floor - 1], &outterButtonGroup::onRequestHandled);
}


void elevatorUI::onUpdateWaitingTIme(double averageWaitingTime)
{
	QString timeInfo = QString::number(averageWaitingTime);
	waitingTimeDisplayer->display(timeInfo);
}






guider::guider() : elevatorNum(5), floorNum(20)
{
	auto gLayout = new QGridLayout(this);

	auto floorNumSetting = new QSpinBox;
	auto elevatorNumSetting = new QSpinBox;
	floorNumSetting->setValue(20);
	elevatorNumSetting->setValue(5);

	auto floorNum = new QLabel("Set the number of floors");
	auto elevatorNum = new QLabel("Set the number of elevators");
	floorNum->setFont(QFont("Microsoft YaHei", 10, 50));
	floorNum->setStyleSheet("color:green");
	elevatorNum->setFont(QFont("Microsoft YaHei", 10, 50));
	elevatorNum->setStyleSheet("color:green");

	gLayout->addWidget(floorNum, 0, 0, 1, 1);
	gLayout->addWidget(floorNumSetting, 0, 1, 1, 1);
	gLayout->addWidget(elevatorNum, 1, 0, 1, 1);
	gLayout->addWidget(elevatorNumSetting, 1, 1, 1, 1);

	auto create = new QPushButton("Create elevator", this);
	create->setFont(QFont("Microsoft YaHei", 10, 50));
	create->setStyleSheet("color:grey");

	gLayout->addWidget(create, 2, 0, 1, 2);


	connect(floorNumSetting, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &guider::setFloorNum);
	connect(elevatorNumSetting, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &guider::setElevatorNum);
	connect(create, &QPushButton::clicked, this, &guider::createElevatorUI);
}

void guider::setFloorNum(int num)
{
	if (num > 0)
		floorNum = num;
}

void guider::setElevatorNum(int num)
{
	if (num > 0)
		elevatorNum = num;
}

void guider::createElevatorUI()
{
	auto eui = new elevatorUI(elevatorNum, floorNum);
	eui->resize(elevatorNum * 140 + 600, floorNum * 40 + 200);
	auto sb = new QScrollArea;
	sb->resize(1000, 700);
	sb->setWidget(eui);
	//sb->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	sb->setWindowTitle("Elevators");
	sb->show();
}
