#pragma once

#include <QtWidgets/QMainWindow>
#include<QInputDialog>
#include<QMessageBox>
#include "myFS2.h"
#include "ui_mainInterface.h"
#include "ui_textEdit.h"

class OS3 : public QMainWindow
{
	Q_OBJECT

public:
	OS3(QWidget *parent = Q_NULLPTR);
	~OS3();

	void updateDir();
	void newFile();
	void newFolder();
	void open(QModelIndex index);
	void removeSlot(bool checked);
	void remove(QListWidgetItem* item);
	void format();
	void back();
	void popMenu(QPoint p);
	void closeEvent(QCloseEvent* event);

private:
	Ui::OS3Class ui;
	FSManager* fsm;
	QListWidgetItem* itemToBeRemove;
	QIcon fileIcon;
	QIcon folderIcon;

	void updateDirPath();
	void updateDirContent();
	void doUpdateDirTree();
	void updateDirTree(myFolder* Root, QTreeWidgetItem* treeItem);

	void openFile(myFile* file);
	void openFolder(myFolder* folder);

	bool isDuplicated(QString name, bool isFile);
};





class fileItem : public QListWidgetItem {
public:
	fileItem(myFile* _file, const QIcon &icon, const QString &text,
		QListWidget *view = nullptr, int type = Type);

	myFile* file;
};

class folderItem : public QListWidgetItem {
public:
	folderItem(myFolder* _folder, const QIcon &icon, const QString &text,
		QListWidget *view = nullptr, int type = Type);

	myFolder* folder;
};