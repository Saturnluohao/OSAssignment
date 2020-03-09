/********************************************************************************
** Form generated from reading UI file 'os2.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OS2_H
#define UI_OS2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OS2Class
{
public:
    QWidget *centralWidget;
    QListWidget *logList;
    QLabel *label;
    QRadioButton *FIFOButton;
    QRadioButton *LRUButton;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *singleButton;
    QPushButton *allButton;
    QLabel *label_6;
    QLabel *label_7;
    QSpinBox *insNum;
    QSpinBox *pageSize;
    QSpinBox *frameNum;
    QLabel *pageErrorNum;
    QLabel *pageErrorRate;
    QPushButton *generateInsSeq;
    QListWidget *insIndex;
    QLabel *label_8;
    QListWidget *insLogicalAddr;
    QLabel *label_9;
    QLabel *label_10;

    void setupUi(QMainWindow *OS2Class)
    {
        if (OS2Class->objectName().isEmpty())
            OS2Class->setObjectName(QString::fromUtf8("OS2Class"));
        OS2Class->resize(956, 452);
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        OS2Class->setFont(font);
        centralWidget = new QWidget(OS2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        logList = new QListWidget(centralWidget);
        logList->setObjectName(QString::fromUtf8("logList"));
        logList->setGeometry(QRect(130, 50, 601, 381));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(380, 10, 71, 20));
        FIFOButton = new QRadioButton(centralWidget);
        FIFOButton->setObjectName(QString::fromUtf8("FIFOButton"));
        FIFOButton->setGeometry(QRect(740, 100, 89, 16));
        LRUButton = new QRadioButton(centralWidget);
        LRUButton->setObjectName(QString::fromUtf8("LRUButton"));
        LRUButton->setGeometry(QRect(800, 100, 89, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(740, 80, 91, 16));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font1.setBold(true);
        font1.setWeight(75);
        label_2->setFont(font1);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(820, 140, 81, 16));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(820, 180, 71, 16));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(820, 220, 131, 16));
        singleButton = new QPushButton(centralWidget);
        singleButton->setObjectName(QString::fromUtf8("singleButton"));
        singleButton->setGeometry(QRect(740, 300, 75, 23));
        allButton = new QPushButton(centralWidget);
        allButton->setObjectName(QString::fromUtf8("allButton"));
        allButton->setGeometry(QRect(840, 300, 75, 23));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(750, 350, 54, 12));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(850, 350, 54, 12));
        insNum = new QSpinBox(centralWidget);
        insNum->setObjectName(QString::fromUtf8("insNum"));
        insNum->setGeometry(QRect(740, 140, 71, 22));
        insNum->setMaximum(10000);
        insNum->setSingleStep(5);
        insNum->setValue(320);
        pageSize = new QSpinBox(centralWidget);
        pageSize->setObjectName(QString::fromUtf8("pageSize"));
        pageSize->setGeometry(QRect(740, 180, 71, 22));
        pageSize->setValue(10);
        frameNum = new QSpinBox(centralWidget);
        frameNum->setObjectName(QString::fromUtf8("frameNum"));
        frameNum->setGeometry(QRect(740, 220, 71, 22));
        frameNum->setValue(4);
        pageErrorNum = new QLabel(centralWidget);
        pageErrorNum->setObjectName(QString::fromUtf8("pageErrorNum"));
        pageErrorNum->setGeometry(QRect(810, 350, 21, 16));
        pageErrorNum->setStyleSheet(QString::fromUtf8("color:rgb(255, 0, 0)"));
        pageErrorRate = new QLabel(centralWidget);
        pageErrorRate->setObjectName(QString::fromUtf8("pageErrorRate"));
        pageErrorRate->setGeometry(QRect(900, 350, 41, 16));
        pageErrorRate->setStyleSheet(QString::fromUtf8("color:rgb(255, 0, 0)"));
        generateInsSeq = new QPushButton(centralWidget);
        generateInsSeq->setObjectName(QString::fromUtf8("generateInsSeq"));
        generateInsSeq->setGeometry(QRect(770, 260, 121, 23));
        insIndex = new QListWidget(centralWidget);
        insIndex->setObjectName(QString::fromUtf8("insIndex"));
        insIndex->setGeometry(QRect(20, 50, 51, 381));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(40, 10, 54, 12));
        insLogicalAddr = new QListWidget(centralWidget);
        insLogicalAddr->setObjectName(QString::fromUtf8("insLogicalAddr"));
        insLogicalAddr->setGeometry(QRect(50, 50, 61, 381));
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 30, 31, 16));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(60, 30, 54, 16));
        OS2Class->setCentralWidget(centralWidget);

        retranslateUi(OS2Class);

        QMetaObject::connectSlotsByName(OS2Class);
    } // setupUi

    void retranslateUi(QMainWindow *OS2Class)
    {
        OS2Class->setWindowTitle(QApplication::translate("OS2Class", "OS2", nullptr));
        label->setText(QApplication::translate("OS2Class", "\351\241\265\351\235\242\350\260\203\345\272\246\346\227\245\345\277\227", nullptr));
        FIFOButton->setText(QApplication::translate("OS2Class", "FIFO", nullptr));
        LRUButton->setText(QApplication::translate("OS2Class", "LRU", nullptr));
        label_2->setText(QApplication::translate("OS2Class", "\350\257\267\351\200\211\346\213\251\347\275\256\346\215\242\347\256\227\346\263\225", nullptr));
        label_3->setText(QApplication::translate("OS2Class", "\351\200\211\346\213\251\346\214\207\344\273\244\346\235\241\346\225\260", nullptr));
        label_4->setText(QApplication::translate("OS2Class", "\351\200\211\346\213\251\351\241\265\351\235\242\345\244\247\345\260\217", nullptr));
        label_5->setText(QApplication::translate("OS2Class", "\351\200\211\346\213\251\345\210\206\351\205\215\347\232\204\345\206\205\345\255\230\345\235\227\346\225\260\351\207\217", nullptr));
        singleButton->setText(QApplication::translate("OS2Class", "\345\215\225\346\214\207\344\273\244\346\211\247\350\241\214", nullptr));
        allButton->setText(QApplication::translate("OS2Class", "\345\205\250\351\203\250\346\211\247\350\241\214", nullptr));
        label_6->setText(QApplication::translate("OS2Class", "\347\274\272\351\241\265\346\225\260\351\207\217\357\274\232", nullptr));
        label_7->setText(QApplication::translate("OS2Class", "\347\274\272\351\241\265\347\216\207\357\274\232", nullptr));
        pageErrorNum->setText(QApplication::translate("OS2Class", "0", nullptr));
        pageErrorRate->setText(QApplication::translate("OS2Class", "0", nullptr));
        generateInsSeq->setText(QApplication::translate("OS2Class", "\344\272\247\347\224\237\346\214\207\344\273\244\350\256\277\351\227\256\345\272\217\345\210\227", nullptr));
        label_8->setText(QApplication::translate("OS2Class", "\346\214\207\344\273\244\345\272\217\345\210\227", nullptr));
        label_9->setText(QApplication::translate("OS2Class", "\345\272\217\345\217\267", nullptr));
        label_10->setText(QApplication::translate("OS2Class", "\351\200\273\350\276\221\345\234\260\345\235\200", nullptr));
    } // retranslateUi

};

namespace Ui {
    class OS2Class: public Ui_OS2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OS2_H
