#pragma once
#include<iostream>
#include<vector>
#include<random>
#include<time.h>
#include<list>
#include<QObject>

using namespace std;

//一个块带有的信息 
struct frame {
	int addr;   //块地址 
	bool validation;   //是否有效 
	frame(int _addr) : addr(_addr), validation(false) {}
	frame(int _addr, bool isValid) :addr(_addr), validation(isValid) {}
};

//页面置换算法类 
class PRA  : public QObject{
	Q_OBJECT

public:
	PRA(int pS, int fQS, int iN); //构造函数 

	void singleFIFO();  //单步执行FIFO 
	void singleLRU();   //单步执行LRU 
	void FIFO();        //全部执行FIFO 
	void LRU();         //全部执行LRU 

signals:
	void status(int addr, bool isError, int pageErrorNum, int currentFrame, int replacedFrame = 0);
private:
	vector<frame> pageTable;  //页表 
	vector<int>insSeq;        //指令访问序列 
	list<int> frameQueue;     //内存块队列 
	int pageSize, frameQueueSize, insNum, currentInsIndex, pageError; //分别为 页面大小、内存块数量、指令数量、当前指令下标、页错误数量 
	bool isFull;  //内存块队列是否已满 

	void setParam(int pS, int fQS, int iN);  //设置参数 
	void init();   //初始化页表等 
	void reset();  //重置各参数 

	friend class OS2;  //设置UI类为友元类 
};


void generatePageTable(vector<frame>& pageTable, int pageSize, int insNum);   //产生页表 
void initFrameQueue(list<frame>& frameQueue, int frameQueueSize);   //初始化内存列表 
int myRand(int left, int right);   //随机函数 
int regularize(int addr);   //规范地址空间 
void generateInstructionSequence(vector<int>& insSeq, int insNum);  //产生指令访问序列 
int getPageIndex(int addr, int pageSize);  //计算逻辑块地址 
int getOffset(int addr, int pageSize);   //计算偏移量 
int getPhysicalAddr(int virtualAddr, vector<frame> pageTable, int pageSize); //计算指令物理地址 
list<int>::iterator getFrameIter(list<int>& frameQueue, int frameAddr);  //返回内存列表迭代器 
