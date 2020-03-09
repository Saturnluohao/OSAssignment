#ifndef OS3HDWHQL_H
#define OS3HDWHQL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include<QMenu>
#include<QAction>

QT_BEGIN_NAMESPACE

class Ui_OS3Class
{
public:
	QWidget *centralWidget;
	QLabel *dirPath;
	QLabel *label_2;
	QListWidget *dirContent;
	QTreeWidget *dirTree;
	QPushButton *newFileButton;
	QPushButton *newFolderButton;
	QPushButton *formatButton;
	QTextBrowser *textBrowser;
	QPushButton *backButton;

	void setupUi(QMainWindow *OS3Class)
	{
		if (OS3Class->objectName().isEmpty())
			OS3Class->setObjectName(QString::fromUtf8("OS3Class"));
		OS3Class->resize(829, 455);
		centralWidget = new QWidget(OS3Class);
		centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
		dirPath = new QLabel(centralWidget);
		dirPath->setObjectName(QString::fromUtf8("dirPath"));
		dirPath->setGeometry(QRect(180, 20, 401, 20));
		QFont font;
		font.setFamily(QString::fromUtf8("Times New Roman"));
		font.setPointSize(10);
		dirPath->setFont(font);
		dirPath->setStyleSheet(QString::fromUtf8("background-color:rgb(85, 255, 255)"));
		label_2 = new QLabel(centralWidget);
		label_2->setObjectName(QString::fromUtf8("label_2"));
		label_2->setGeometry(QRect(110, 20, 54, 20));
		dirContent = new QListWidget(centralWidget);
		dirContent->setObjectName(QString::fromUtf8("dirContent"));
		dirContent->setGeometry(QRect(180, 50, 401, 331));
		dirContent->setFont(font);
		dirTree = new QTreeWidget(centralWidget);
		QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
		__qtreewidgetitem->setText(0, QString::fromUtf8("1"));
		dirTree->setHeaderItem(__qtreewidgetitem);
		dirTree->setObjectName(QString::fromUtf8("dirTree"));
		dirTree->setGeometry(QRect(0, 50, 171, 331));
		newFileButton = new QPushButton(centralWidget);
		newFileButton->setObjectName(QString::fromUtf8("newFileButton"));
		newFileButton->setGeometry(QRect(220, 400, 75, 23));
		newFolderButton = new QPushButton(centralWidget);
		newFolderButton->setObjectName(QString::fromUtf8("newFolderButton"));
		newFolderButton->setGeometry(QRect(340, 400, 75, 23));
		formatButton = new QPushButton(centralWidget);
		formatButton->setObjectName(QString::fromUtf8("formatButton"));
		formatButton->setGeometry(QRect(460, 400, 75, 23));
		textBrowser = new QTextBrowser(centralWidget);
		textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
		textBrowser->setGeometry(QRect(590, 51, 231, 331));
		backButton = new QPushButton(centralWidget);
		backButton->setObjectName(QString::fromUtf8("backButton"));
		backButton->setGeometry(QRect(600, 20, 31, 23));
		QFont font1;
		font1.setPointSize(10);
		backButton->setFont(font1);
		backButton->setStyleSheet(QString::fromUtf8("background-color:rgb(255, 255, 127)"));
		OS3Class->setCentralWidget(centralWidget);

		retranslateUi(OS3Class);

		QMetaObject::connectSlotsByName(OS3Class);
	} // setupUi

	void retranslateUi(QMainWindow *OS3Class)
	{
		OS3Class->setWindowTitle(QApplication::translate("OS3Class", "OS3", nullptr));
		dirPath->setText(QString());
		label_2->setText(QApplication::translate("OS3Class", "\345\275\223\345\211\215\347\233\256\345\275\225\357\274\232", nullptr));
		newFileButton->setText(QApplication::translate("OS3Class", "\346\226\260\345\273\272\346\226\207\344\273\266", nullptr));
		newFolderButton->setText(QApplication::translate("OS3Class", "\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
		formatButton->setText(QApplication::translate("OS3Class", "\346\240\274\345\274\217\345\214\226", nullptr));
		textBrowser->setHtml(QApplication::translate("OS3Class", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
			"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
			"p, li { white-space: pre-wrap; }\n"
			"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; color:#ff0000;\">\350\257\264\346\230\216\357\274\232</span></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\346\240\274\345\274\217\345\214\226\357\274\232\347\202\271\345\207\273\342\200\234\346\240\274\345\274\217\345\214\226\342\200\235\346\214\211\351\222\256<"
			"/span></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#ffaa00;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\346\226\260\345\273\272\346\226\207\344\273\266\357\274\232\347\202\271\345\207\273\342\200\234\346\226\260\345\273\272\346\226\207\344\273\266\342\200\235\346\214\211\351\222\256\357\274\214</span></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">          \346\210\226\345\217\263\351\224\256-&gt;\342\200\234New File\342\200\235</span></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#ffaa00;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px"
			"; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\345\210\240\351\231\244\346\226\207\344\273\266\357\274\232\345\217\263\351\224\256-&gt;\342\200\234\345\210\240\351\231\244\342\200\235</span></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#ffaa00;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271\357\274\232\347\202\271\345\207\273\342\200\234\346\226\260\345\273\272\346\226\207\344\273\266\345\244\271\342\200\235\346\214\211\351\222\256\357\274\214</span></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">            \346\210\226\345\217\263\351\224\256-&"
			"gt;\342\200\234New Folder\342\200\235</span></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#ffaa00;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271\357\274\232\345\217\263\351\224\256-&gt;\342\200\234\345\210\240\351\231\244\342\200\235</span></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#ffaa00;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\346\211\223\345\274\200\346\226\207\344\273\266/\350\257\273\346\226\207\344\273\266\357\274\232\345\217\214\345\207\273\346\226\207\344\273\266</sp"
			"an></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#ffaa00;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\346\211\223\345\274\200\346\226\207\344\273\266\345\244\271\357\274\232\345\217\214\345\207\273\346\226\207\344\273\266\345\244\271</span></p>\n"
			"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; color:#ffaa00;\"><br /></p>\n"
			"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" color:#ffaa00;\">\345\206\231\345\205\245\346\226\207\344\273\266\357\274\232\347\202\271\345\207\273\342\200\234\344\277\235\345\255\230\342\200\235\346\214\211\351\222\256</span></p></body></html>", nullptr));
		backButton->setText(QApplication::translate("OS3Class", "\350\277\224\345\233\236", nullptr));
	} // retranslateUi

};

namespace Ui {
	class OS3Class : public Ui_OS3Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OS3HDWHQL_H