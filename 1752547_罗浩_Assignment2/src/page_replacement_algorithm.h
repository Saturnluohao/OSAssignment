#pragma once
#include<iostream>
#include<vector>
#include<random>
#include<time.h>
#include<list>
#include<QObject>

using namespace std;

//һ������е���Ϣ 
struct frame {
	int addr;   //���ַ 
	bool validation;   //�Ƿ���Ч 
	frame(int _addr) : addr(_addr), validation(false) {}
	frame(int _addr, bool isValid) :addr(_addr), validation(isValid) {}
};

//ҳ���û��㷨�� 
class PRA  : public QObject{
	Q_OBJECT

public:
	PRA(int pS, int fQS, int iN); //���캯�� 

	void singleFIFO();  //����ִ��FIFO 
	void singleLRU();   //����ִ��LRU 
	void FIFO();        //ȫ��ִ��FIFO 
	void LRU();         //ȫ��ִ��LRU 

signals:
	void status(int addr, bool isError, int pageErrorNum, int currentFrame, int replacedFrame = 0);
private:
	vector<frame> pageTable;  //ҳ�� 
	vector<int>insSeq;        //ָ��������� 
	list<int> frameQueue;     //�ڴ����� 
	int pageSize, frameQueueSize, insNum, currentInsIndex, pageError; //�ֱ�Ϊ ҳ���С���ڴ��������ָ����������ǰָ���±ꡢҳ�������� 
	bool isFull;  //�ڴ������Ƿ����� 

	void setParam(int pS, int fQS, int iN);  //���ò��� 
	void init();   //��ʼ��ҳ��� 
	void reset();  //���ø����� 

	friend class OS2;  //����UI��Ϊ��Ԫ�� 
};


void generatePageTable(vector<frame>& pageTable, int pageSize, int insNum);   //����ҳ�� 
void initFrameQueue(list<frame>& frameQueue, int frameQueueSize);   //��ʼ���ڴ��б� 
int myRand(int left, int right);   //������� 
int regularize(int addr);   //�淶��ַ�ռ� 
void generateInstructionSequence(vector<int>& insSeq, int insNum);  //����ָ��������� 
int getPageIndex(int addr, int pageSize);  //�����߼����ַ 
int getOffset(int addr, int pageSize);   //����ƫ���� 
int getPhysicalAddr(int virtualAddr, vector<frame> pageTable, int pageSize); //����ָ�������ַ 
list<int>::iterator getFrameIter(list<int>& frameQueue, int frameAddr);  //�����ڴ��б������ 
