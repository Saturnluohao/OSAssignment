#include "myFS2.h"

myFolder* Root;
vector<myFile*> allFiles;
vector<myFolder*> allFolders;
short firstEmptyBlock;
diskBlock** blockList;
short* FAT;
vector<short> removedFolderLocations;




diskBlock::diskBlock(short nFB){
	nextFreeBlock = nFB;
	for (int i = 0; i < BLOCK_SIZE; i++)
		data[i] = 0;
}




myFile::myFile(short parent, const char* _name){
	int copyLen = strlen(_name) >= NAME_LENGTH ? NAME_LENGTH : strlen(_name);
	if (strlen(_name) == 0) {
		myStrCpy(name, DEFAULT_FILE_NAME, strlen(DEFAULT_FILE_NAME));
	}
	else {
		myStrCpy(name, _name, copyLen);
	}
	startBlock = -1;
	sizeByBlock = 0;
	parentFolder = parent;
}

myFile::~myFile(){
	if (startBlock == -1)
		return;
	short current = startBlock, next = FAT[current];
	while (current != -1 && next != -1) {
		blockList[current]->nextFreeBlock = next;
		current = next;
		next = FAT[next];
	}
	blockList[current]->nextFreeBlock = firstEmptyBlock;
	firstEmptyBlock = startBlock;
}

void myFile::read(char*& str){
	short current = startBlock;
	string content;
	while (current >= 0) {
		content.append(blockList[current]->data);
		current = FAT[current];
	}
	str = new char[content.length() + 1];
	myStrCpy(str, content.c_str(), content.length());
}

void myFile::write(string content) {
	if (content.empty())
		return;
	int block_needed = (content.length() - 1) / (BLOCK_SIZE - 1) + 1, extra = content.length() % (BLOCK_SIZE - 1);

	if (startBlock == -1)
		extend(startBlock);
	if (extra == 0)
		extra = BLOCK_SIZE - 1;
	short current = startBlock;
	for (short i = 0; i < block_needed - 1; i++) {
		myStrCpy(blockList[current]->data, content.substr(i * (BLOCK_SIZE - 1), (BLOCK_SIZE - 1)).c_str(), BLOCK_SIZE - 1);
		if (FAT[current] == -1)
			current = extend(current);
		else
			current = FAT[current];
	}
	myStrCpy(blockList[current]->data, content.substr((BLOCK_SIZE - 1) * (block_needed - 1), extra).c_str(), extra);

	relink(current, sizeByBlock - block_needed - 1);
	sizeByBlock = block_needed;
}

short myFile::extend(short finalBLock) {
	if (finalBLock == -1) {
		startBlock = firstEmptyBlock;
	}
	else {
		FAT[finalBLock] = firstEmptyBlock;
	}
	finalBLock = firstEmptyBlock;
	FAT[finalBLock] = -1;
	firstEmptyBlock = blockList[firstEmptyBlock]->nextFreeBlock;
	sizeByBlock++;
	return finalBLock;
}

void myFile::relink(short finalBlock, short len) {
	if (len <= 0)
		return;
	short new_first_empty_block = FAT[finalBlock];
	short current = new_first_empty_block;
	FAT[finalBlock] = -1;

	for (int i = 0; i < len; i++) {
		blockList[current]->nextFreeBlock = FAT[current];
		current = FAT[current];
	}
	blockList[current]->nextFreeBlock = firstEmptyBlock;
	firstEmptyBlock = new_first_empty_block;
}






myFolder::myFolder(short parent, const char* _name){
	int copyLen = strlen(_name) >= NAME_LENGTH ? NAME_LENGTH : strlen(_name);
	if (strlen(_name) == 0) {
		myStrCpy(name, DEFAULT_FOLDER_NAME, strlen(DEFAULT_FOLDER_NAME));
	}
	else {
		myStrCpy(name, _name, copyLen);
	}
	if (removedFolderLocations.empty())
		index = allFolders.size();
	else {
		index = removedFolderLocations.back();
		removedFolderLocations.pop_back();
	}
	parentFolder = parent;
}

myFolder::~myFolder(){
	//clearFolder(this);
}

void myFolder::setInfo(short parent, const char* _name, short _index){
	parentFolder = parent;
	myStrCpy(name, _name, NAME_LENGTH);
	index = _index;
}

void myFolder::addFile(myFile* file){
	fileList.push_back(file);
}

void myFolder::addFolder(myFolder* folder){
	folderList.push_back(folder);
}







FSManager::FSManager(){
	restoreFS();
	currentPath.push_back(root());
}

FSManager::~FSManager(){
	storeFS();
	delete[] FAT;
	for (int i = 0; i < BLOCK_NUM; i++) {
		delete blockList[i];
	}
	delete[] blockList;
}

myFile* FSManager::newFile(myFolder* parent, string name){
	myFile* new_file = new myFile(parent->index, name.c_str());
	parent->fileList.push_back(new_file);
	allFiles.push_back(new_file);
	return new_file;
}

myFolder* FSManager::newFolder(myFolder* parent, string name){
	myFolder* new_folder = new myFolder(parent->index, name.c_str());
	parent->folderList.push_back(new_folder);
	allFolders.push_back(new_folder);
	return new_folder;
}

void FSManager::format(){
	clearFolder(Root);
}

void FSManager::removeFile(myFolder* parent, myFile* file){
	for (int i = 0; i < parent->fileList.size(); i++) {
		if (parent->fileList[i] == file) {
			parent->fileList.erase(parent->fileList.begin() + i);
			delete file;
			break;
		}
	}
}

void FSManager::removeFolder(myFolder* parent, myFolder* folder){
	for (int i = 0; i < parent->folderList.size(); i++) {
		if (parent->folderList[i] == folder) {
			removedFolderLocations.push_back(parent->folderList[i]->index);
			parent->folderList.erase(parent->folderList.begin() + i);
			clearFolder(folder);
			break;
		}
	}
}

string FSManager::getDirPath() {
	string path;
	for (int i = 0; i < currentPath.size(); i++) {
		path.append(currentPath[i]->name);
		if (i > 0)
			path.append("/");
	}
	return path;
}





void initBlockList(diskBlock** bL, int blockNum){
	for (int i = 0; i < blockNum; i++) {
		bL[i] = new diskBlock(i+1);
	}
	bL[blockNum - 1]->nextFreeBlock = -1;
}

void initFAT(short* fat, int size){
	for (int i = 0; i < size; i++) {
		fat[i] = -1;
	}
}






void clearFolder(myFolder* folder) {
	for (auto i : folder->fileList) {
		delete i;
	}
	for (auto i : folder->folderList) {
		clearFolder(i);
	}
	delete folder;
}

char* myStrCpy(char* des, const char* src, int len){
	char* r = des;
	int index = 0;
	assert((des != NULL) && (src != NULL));
	while (index++ < len) {
		*r++ = *src++;
	};
	des[len] = '\0';
	return des;
}

myFolder* root() {
	return Root;
}

void storeDiskBlockList(){
	FILE* blockFile = fopen("FSData/diskBlockList", "wb+");
	for (int i = 0; i < BLOCK_NUM; i++) {
		fwrite(blockList[i], sizeof(diskBlock), 1, blockFile);
	}
	fwrite(&firstEmptyBlock, sizeof(short), 1, blockFile);
	fclose(blockFile);
}

void storeFAT(){
	FILE* FATFile = fopen("FSData/FATFile", "wb+");
	fwrite(FAT, sizeof(short), BLOCK_NUM, FATFile);
	fclose(FATFile);
}

void storeMyFiles(){
	FILE* myFiles = fopen("FSData/myFiles", "wb+");
	for (int i = 0; i < allFiles.size(); i++) {
		fwrite(allFiles[i], sizeof(myFile), 1, myFiles);
	}
	fclose(myFiles);
}

void storeMyFolders(){
	FILE* myFolders = fopen("FSData/myFolders", "wb+");
	for (auto i : allFolders) {
		fwrite(&i->parentFolder, sizeof(short), 1, myFolders);
		fwrite(i->name, NAME_LENGTH, 1, myFolders);
		fwrite(&i->index, sizeof(short), 1, myFolders);
	}
	fclose(myFolders);
}

void storeRemovedFolders(){
	FILE* removedFolders = fopen("FSData/removedFolders", "wb+");
	for (int i = 0; i < removedFolderLocations.size(); i++) {
		fwrite(&removedFolderLocations[i], sizeof(short), 1, removedFolders);
	}
	fclose(removedFolders);
}

void storeFS() {
	storeDiskBlockList();
	storeFAT();
	storeMyFiles();
	storeMyFolders();
	storeRemovedFolders();
}





void restoreDiskBlockList(FILE* blockFile) {
	blockList = new diskBlock*[BLOCK_NUM];
	for (int i = 0; i < BLOCK_NUM; i++) {
		diskBlock* temp = new diskBlock(0);
		if(fread(temp, sizeof(diskBlock), 1, blockFile))
			blockList[i] = temp;
	}
	fread(&firstEmptyBlock, sizeof(short), 1, blockFile);
	fclose(blockFile);
}

void restoreFAT(FILE* FATFile) {
	FAT = new short[BLOCK_NUM];
	fread(FAT, sizeof(short), BLOCK_NUM, FATFile);
	fclose(FATFile);
}

void restoreMyFiles(FILE* myFiles) {
	myFile* temp = new myFile(-1, "");
	while(fread(temp, sizeof(myFile), 1, myFiles)) {
		allFiles.push_back(temp);
		temp = new myFile(-1, "");
	}
	delete temp;
	fclose(myFiles);
}

void restoreMyFolders(FILE* myFolders) {
	myFolder* temp = new myFolder(-1, "");
	short parent, index;
	char name[NAME_LENGTH];
	while(fread(&parent, sizeof(short), 1, myFolders)) {
		fread(name, NAME_LENGTH, 1, myFolders);
		fread(&index, sizeof(short), 1, myFolders);
		temp->setInfo(parent, name, index);
		allFolders.push_back(temp);
		temp = new myFolder(-1, "");
	}
	clearFolder(temp);
	fclose(myFolders);
}

void restoreRemovedFolders(FILE* removedFolders) {
	short temp;
	while(fread(&temp, sizeof(short), 1, removedFolders)) {
		removedFolderLocations.push_back(temp);
	}
	fclose(removedFolders);
}

void restoreDirectory(){
	for (auto i : allFolders){
		if (i->parentFolder >= 0) {
			allFolders[i->parentFolder]->addFolder(i);
		}
		else if(i->parentFolder == -1)
			Root = i;
	}
	for (auto i : allFiles) {
		if (i->parentFolder >= 0) {
			allFolders[i->parentFolder]->addFile(i);
		}
	}
}

void restoreFS(){
	FAT = new short[BLOCK_NUM];
	blockList = new diskBlock*[BLOCK_NUM];
	FILE* blockFile = fopen("FSData/diskBlockList", "rb");
	FILE* FATFile = fopen("FSData/FATFile", "rb");
	FILE* myFiles = fopen("FSData/myFiles", "rb");
	FILE* myFolders = fopen("FSData/myFolders", "rb");
	FILE* removedFolders = fopen("FSData/removedFolders", "rb");
	if (blockFile && FATFile && myFiles && myFolders) {
		restoreDiskBlockList(blockFile);
		restoreFAT(FATFile);
		restoreMyFiles(myFiles);
		restoreMyFolders(myFolders);
		restoreRemovedFolders(removedFolders);
		restoreDirectory();
	}
	else {
		Root = new myFolder(-1, "/");
		allFolders.push_back(Root);
		firstEmptyBlock = 0;
		initBlockList(blockList, BLOCK_NUM);
		initFAT(FAT, BLOCK_NUM);
	}
}
