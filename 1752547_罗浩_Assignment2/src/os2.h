#pragma once

#include <QtWidgets/QMainWindow>
#include<QMessageBox>
#include "page_replacement_algorithm.h"
#include "ui_os2.h"

class OS2 : public QMainWindow
{
	Q_OBJECT

public:
	OS2(QWidget *parent = Q_NULLPTR);

	void updatePraParam();
	void executeSingle();
	void executeAll();
	void updateWidgets(int addr, bool isError, int pageErrorNum, int currentFrame, int replacedFrame);

private:
	Ui::OS2Class ui;
	PRA* pra;

	void toggleAbility(bool isEnable);
	void updateInsSeq();
};
