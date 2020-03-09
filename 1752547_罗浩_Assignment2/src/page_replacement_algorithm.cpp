#include "page_replacement_algorithm.h"

//一个块带有的信息 
void generatePageTable(vector<frame>& pageTable, int pageSize, int insNum) {
	unsigned seed = clock();
	int pageNum = (insNum - 1) / pageSize + 1;

	if (!pageTable.empty())
		pageTable.clear();
	for (int i = 0; i < pageNum; i++) {
		pageTable.push_back(frame(i));
	}
	shuffle(pageTable.begin(), pageTable.end(), default_random_engine(seed));
}

//初始化内存列表 
void initFrameQueue(list<int>& frameQueue, int frameQueueSize) {
	if (!frameQueue.empty())
		frameQueue.clear();
	for (int i = 0; i < frameQueueSize; i++) {
		frameQueue.push_back(-1);
	}
}

//产生一定范围的随机数 
int myRand(int left, int right) {
	if (right < left)
		return -1;
	return left + rand() % (right - left + 1);
}

//规范地址空间 
int regularize(int addr) {
	if (addr < 0)
		return 0;
	else if (addr > 319)
		return 319;
	else
		return addr;
}

//产生指令访问序列 
void generateInstructionSequence(vector<int>& insSeq, int insNum) {
	int status = 0, left = 0, right = insNum;
	srand((unsigned)time(NULL));

	if (!insSeq.empty())
		insSeq.clear();
	for (int i = 0; i < insNum; i++) {
		switch (status) {
		case 0:
			right = myRand(0, 319);
			insSeq.push_back(regularize(right));
			status++;
			break;
		case 1:
			insSeq.push_back(regularize(right + 1));
			status++;
			break;
		case 2:
			left = myRand(0, right - 1);
			insSeq.push_back(regularize(left));
			status++;
			break;
		case 3:
			insSeq.push_back(regularize(left + 1));
			status++;
			break;
		case 4:
			right = myRand(left + 2, 319);
			insSeq.push_back(regularize(right));
			status++;
			break;
		case 5:
			insSeq.push_back(regularize(right + 1));
			status = 0;
		}
	}
}

//计算逻辑快地址 
int getPageIndex(int addr, int pageSize) {
	if (pageSize <= 0)
		return 0;
	return addr / pageSize;
}

//计算偏移量 
int getOffset(int addr, int pageSize) {
	if (pageSize <= 0)
		return 0;
	return addr % pageSize;
}

//计算指令物理地址 
int getPhysicalAddr(int virtualAddr, vector<frame> pageTable, int pageSize) {
	int index = getPageIndex(virtualAddr, pageSize);
	int offset = getOffset(virtualAddr, pageSize);
	return pageTable[index].addr * pageSize + offset;
}

//在内存列表中查找特定块位置 
list<int>::iterator getFrameIter(list<int>& frameQueue, int frameAddr) {
	for (auto i = frameQueue.begin(); i != frameQueue.end(); i++) {
		if (*i == frameAddr)
			return i;
	}
	return frameQueue.end();
}




//构造函数 
PRA::PRA(int pS, int fQS, int iN) {
	pageSize = pS;
	frameQueueSize = fQS;
	insNum = iN;
	isFull = false;
	pageError = 0;
	currentInsIndex = 0;
	initFrameQueue(frameQueue, frameQueueSize);
	generatePageTable(pageTable, pageSize, insNum);
	generateInstructionSequence(insSeq, insNum);
}

//单步执行FIFO 
void PRA::singleFIFO() {
	int currentInsAddr = insSeq[currentInsIndex];
	int pageIndex = getPageIndex(currentInsAddr, pageSize);

	if (pageTable[pageIndex].validation) {             //有效则直接访问 
		emit(status(getPhysicalAddr(currentInsAddr, pageTable, pageSize), false, pageError, pageIndex));
	}
	else if (!isFull) {                //内存列表没空时加到空位置 
		auto emptyAddr = getFrameIter(frameQueue, -1);
		if (emptyAddr != frameQueue.end()) {
			*emptyAddr = pageIndex;
			pageTable[pageIndex].validation = true;
			pageError++;
			emit(status(getPhysicalAddr(currentInsAddr, pageTable, pageSize), false, pageError, pageIndex));
		}
		else
			isFull = true;
	}
	else {                     //置换队列头块 
		pageError++;
		int frontAddr = frameQueue.front();
		if (frontAddr >= 0 && pageTable[frontAddr].validation)
			pageTable[frontAddr].validation = false;
		frameQueue.pop_front();
		frameQueue.push_back(pageIndex);
		pageTable[pageIndex].validation = true;
		emit(status(getPhysicalAddr(currentInsAddr, pageTable, pageSize), true, pageError, pageIndex, frontAddr));
	}
	currentInsIndex++;
}

//单步执行LRU 
void PRA::singleLRU() {
	int currentInsAddr = insSeq[currentInsIndex];
	int pageIndex = getPageIndex(currentInsAddr, pageSize);

	if (pageTable[pageIndex].validation) {             //有效则直接访问，并将被访问的块提到队前 
		cout << getPhysicalAddr(currentInsAddr, pageTable, pageSize) << endl;
		auto frameIter = getFrameIter(frameQueue, pageIndex);
		if (frameIter != frameQueue.end()) {
			frameQueue.erase(frameIter);
			frameQueue.push_front(pageIndex);
		}
		emit(status(getPhysicalAddr(currentInsAddr, pageTable, pageSize), false, pageError, pageIndex));
	}
	else if (!isFull) {         //内存列表没空时加到空位置 
		auto emptyAddr = getFrameIter(frameQueue, -1);
		if (emptyAddr != frameQueue.end()) {
			*emptyAddr = pageIndex;
			pageTable[pageIndex].validation = true;
			pageError++;
			emit(status(getPhysicalAddr(currentInsAddr, pageTable, pageSize), false, pageError, pageIndex));
		}
		else
			isFull = true;
	}
	else {                     //置换队列尾块
		pageError++;
		int backAddr = frameQueue.back();
		if (backAddr >= 0 && pageTable[backAddr].validation)
			pageTable[backAddr].validation = false;
		frameQueue.pop_back();
		frameQueue.push_back(pageIndex);
		pageTable[pageIndex].validation = true;
		emit(status(getPhysicalAddr(currentInsAddr, pageTable, pageSize), true, pageError, pageIndex, backAddr));
	}
	currentInsIndex++;
}

//全部执行FIFO 
void PRA::FIFO() {
	for (int i = currentInsIndex; i < insNum; i++) {
		singleFIFO();
	}
	cout << "The FIFO page-error rate is " << pageError / 3.2 << "%";
	pageError = 0;
}

//全部执行LRU 
void PRA::LRU() {
	for (int i = currentInsIndex; i < insNum; i++) {
		singleLRU();
	}
	cout << "The LRU page-error rate is " << pageError / 3.2 << "%";
	pageError = 0;
}

//设置参数 
void PRA::setParam(int pS, int fQS, int iN){
	pageSize = pS;
	frameQueueSize = fQS;
	insNum = iN;
	init();
}

//初始化页表等 
void PRA::init() {
	initFrameQueue(frameQueue, frameQueueSize);
	generatePageTable(pageTable, pageSize, insNum);
	generateInstructionSequence(insSeq, insNum);
}

//重置各参数 
void PRA::reset(){
	pageError = 0;
	currentInsIndex = 0;
	isFull = false;
}
