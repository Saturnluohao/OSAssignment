#pragma once
#include<vector>
#include<string>
#include<stdio.h>
#include <cassert>

//#pragma warning(disable : 4996)

#define BLOCK_NUM 600
#define BLOCK_SIZE 512
#define NAME_LENGTH 16

static const char* DEFAULT_FILE_NAME = "NewFile";
static const char* DEFAULT_FOLDER_NAME = "NewFolder";

using namespace std;

class diskBlock {
public:
	char data[BLOCK_SIZE];
	short nextFreeBlock;
	diskBlock(short nFB);
};



class myFile {
public:
	myFile(short parent, const char* name = DEFAULT_FILE_NAME);
	~myFile();
	void read(char*& str);
	void write(string content);

	friend class FSManager;
	friend class OS3;
	friend void restoreDirectory();

private:
	char name[NAME_LENGTH];
	short parentFolder;
	short sizeByBlock;
	short startBlock;

	short extend(short finalBlock);
	void relink(short first, short len);
};



class myFolder {
public:
	myFolder(short parent, const char* _name = DEFAULT_FOLDER_NAME);
	~myFolder();
	void setInfo(short parent, const char* name, short index);
	void addFile(myFile* file);
	void addFolder(myFolder* folder);

	short parentFolder;

	friend class FSManager;
	friend class OS3;
	friend void storeMyFolders();
	friend void clearFolder(myFolder* folder);
private:
	char name[NAME_LENGTH];
	short index;
	vector<myFile*> fileList;
	vector<myFolder*> folderList;
};



class FSManager {
public:
	FSManager();
	~FSManager();
	myFile* newFile(myFolder* parent, string name);
	myFolder* newFolder(myFolder* parent, string name);
	void format();
	void removeFile(myFolder* parent, myFile* file);
	void removeFolder(myFolder* parent, myFolder* folder);
	string getDirPath();

	vector<myFolder*> currentPath;
};



void initBlockList(diskBlock** bL, int blockNum);
void initFAT(short* fat, int size);


void clearFolder(myFolder* folder);
char* myStrCpy(char* des, const char* src, int len);
myFolder* root();

void storeDiskBlockList();
void storeFAT();
void storeMyFiles();
void storeMyFolders();
void storeRemovedFolders();
void storeFS();

void restoreDiskBlockList(FILE*);
void restoreFAT(FILE*);
void restoreMyFiles(FILE*);
void restoreMyFolders(FILE*);
void restoreRemovedFolders(FILE*);
void restoreDirectory();
void restoreFS();