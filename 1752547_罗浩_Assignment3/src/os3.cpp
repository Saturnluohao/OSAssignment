#include "os3.h"

OS3::OS3(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	fsm = new FSManager;
	fileIcon = QIcon("icons/file.ico");
	folderIcon = QIcon("icons/folder.ico");
	connect(ui.newFileButton, &QPushButton::clicked, this, &OS3::newFile);
	connect(ui.newFolderButton, &QPushButton::clicked, this, &OS3::newFolder);
	connect(ui.formatButton, &QPushButton::clicked, this, &OS3::format);
	connect(ui.dirContent, &QListWidget::doubleClicked, this, &OS3::open);
	connect(ui.backButton, &QPushButton::clicked, this, &OS3::back);
	ui.dirContent->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.dirContent, &QListWidget::customContextMenuRequested, this, &OS3::popMenu);
	updateDirPath();
	updateDirContent();
	doUpdateDirTree();
	ui.dirTree->setHeaderHidden(true);
}

OS3::~OS3() {
	delete fsm;
}



void OS3::updateDirPath() {
	ui.dirPath->setText(QString::fromStdString(fsm->getDirPath()));
}

void OS3::updateDirContent() {
	auto currentFolder = fsm->currentPath.back();
	ui.dirContent->clear();
	for (auto i : currentFolder->folderList) {
		auto item = new folderItem(i, folderIcon, i->name);
		ui.dirContent->addItem(item);
	}
	for (auto i : currentFolder->fileList) {
		auto item = new fileItem(i, fileIcon, i->name);
		ui.dirContent->addItem(item);
	}
}

void OS3::doUpdateDirTree(){
	ui.dirTree->clear();
	auto treeItem = new QTreeWidgetItem(ui.dirTree, QStringList(QString("/")));
	updateDirTree(root(), treeItem);
	ui.dirTree->expandAll();
}

void OS3::updateDirTree(myFolder* Root, QTreeWidgetItem* treeItem) {
	for (auto i : Root->folderList) {
		auto newTreeItem = new QTreeWidgetItem(treeItem, QStringList(QString(i->name)));
		newTreeItem->setIcon(0, folderIcon);
		updateDirTree(i, newTreeItem);
	}
	for (auto i : Root->fileList) {
		auto newTreeItem = new QTreeWidgetItem(treeItem, QStringList(QString(i->name)));
		newTreeItem->setIcon(0, fileIcon);
	}
}

void OS3::updateDir() {
	updateDirPath();
	updateDirContent();
}

void OS3::newFile() {
	bool isOK;
	QString name = QInputDialog::getText(this, "New File", "Input file name", QLineEdit::Normal, "", &isOK);
	if (isOK) {
		if (name.isEmpty())
			QMessageBox::information(NULL, "WARNING", "File name can't be empty", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		else if (isDuplicated(name, true))
			QMessageBox::information(NULL, "WARNING", "File name can't be duplicated", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		else
			fsm->newFile(fsm->currentPath.back(), name.toStdString());

	}
	updateDir();
	doUpdateDirTree();
}

void OS3::newFolder() {
	bool isOK;
	QString name = QInputDialog::getText(this, "New Folder", "Input folder name", QLineEdit::Normal, "", &isOK);
	if (isOK) {
		if (name.isEmpty())
			QMessageBox::information(NULL, "WARNING", "Folder name can't be empty", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		else if (isDuplicated(name, false))
			QMessageBox::information(NULL, "WARNING", "Folder name can't be duplicated", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		else
			fsm->newFolder(fsm->currentPath.back(), name.toStdString());
	}
	updateDir();
	doUpdateDirTree();
}

void OS3::open(QModelIndex index) {
	auto item = ui.dirContent->item(index.row());
	if (index.row() >= fsm->currentPath.back()->folderList.size())
		openFile(((fileItem*)item)->file);
	else
		openFolder(((folderItem*)item)->folder);
}

void OS3::removeSlot(bool checked){
	if (itemToBeRemove != NULL)
		remove(itemToBeRemove);
}

void OS3::openFile(myFile* file) {
	if (file == NULL)
		return;
	auto textEdit = new textEditer(file->name, file);
	textEdit->show();
}

void OS3::openFolder(myFolder* folder) {
	if (folder == NULL)
		return;
	fsm->currentPath.push_back(folder);
	updateDir();
}

bool OS3::isDuplicated(QString name, bool isFile) {
	if (isFile) {
		for (auto i : fsm->currentPath.back()->fileList) {
			if (i->name == name)
				return true;
		}
	}
	else {
		for (auto i : fsm->currentPath.back()->folderList) {
			if (i->name == name)
				return true;
		}
	}
	return false;
}

void OS3::remove(QListWidgetItem * item) {
	auto index = ui.dirContent->row(item);
	auto myItem = ui.dirContent->item(index);
	if (index >= fsm->currentPath.back()->folderList.size())
		fsm->removeFile(fsm->currentPath.back(), ((fileItem*)item)->file);
	else
		fsm->removeFolder(fsm->currentPath.back(), ((folderItem*)item)->folder);
	updateDir();
	doUpdateDirTree();
}

void OS3::format() {
	fsm->format();
	updateDir();
}

void OS3::back() {
	if (fsm->currentPath.size() == 1)
		return;
	fsm->currentPath.pop_back();
	updateDir();
}

void OS3::popMenu(QPoint p){
	QMenu* menu = new QMenu(this);
	QAction *removeAction = new QAction(tr("Delete"), this);
	QAction *newFile = new QAction(tr("New File"), this);
	QAction *newFolder = new QAction(tr("New Folder"), this);

	itemToBeRemove = ui.dirContent->itemAt(p);
	if (itemToBeRemove != NULL) {
		menu->addAction(removeAction);
		connect(removeAction, &QAction::triggered, this, &OS3::removeSlot);
	}
	else {
		menu->addAction(newFile);
		menu->addAction(newFolder);
		connect(newFile, &QAction::triggered, this, &OS3::newFile);
		connect(newFolder, &QAction::triggered, this, &OS3::newFolder);
	}
	menu->exec(QCursor::pos());
}

void OS3::closeEvent(QCloseEvent * event){
	//storeFS();
}







fileItem::fileItem(myFile * _file, const QIcon & icon, const QString & text, QListWidget * view, int type) : QListWidgetItem(icon, text, view, type) {
	file = _file;
}

folderItem::folderItem(myFolder * _folder, const QIcon & icon, const QString & text, QListWidget * view, int type) : QListWidgetItem(icon, text, view, type) {
	folder = _folder;
}
