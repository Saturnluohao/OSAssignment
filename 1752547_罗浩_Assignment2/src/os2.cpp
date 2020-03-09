#include "os2.h"

OS2::OS2(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.FIFOButton->setChecked(true);
	pra = new PRA(10, 4, 320);

	toggleAbility(true);
	this->setWindowTitle("Page Replacement Algorithm");

	connect(ui.generateInsSeq, &QPushButton::clicked, this, &OS2::updatePraParam);
	connect(ui.singleButton, &QPushButton::clicked, this, &OS2::executeSingle);
	connect(ui.allButton, &QPushButton::clicked, this, &OS2::executeAll);
	connect(pra, &PRA::status, this, &OS2::updateWidgets);
}

void OS2::updatePraParam() {
	if (pra == NULL)
		pra = new PRA(ui.pageSize->value(), ui.frameNum->value(), ui.insNum->value());
	else {
		pra->setParam(ui.pageSize->value(), ui.frameNum->value(), ui.insNum->value());
	}
	updateInsSeq();
	ui.logList->clear();
	ui.pageErrorNum->setText("0");
	ui.pageErrorRate->setText("0");
	toggleAbility(false);
	QMessageBox::about(this, "Tip", "Instruction Sequence generated successfully!");
	pra->reset();
}

void OS2::executeSingle(){
	if (ui.FIFOButton->isChecked()) {
		pra->singleFIFO();
	}
	else {
		pra->singleLRU();
	}
}

void OS2::executeAll(){
	if (ui.FIFOButton->isChecked()) {
		pra->FIFO();
	}
	else {
		pra->LRU();
	}
	toggleAbility(true);
}

void OS2::updateWidgets(int addr, bool isError, int pageErrorNum, int currentFrame, int replacedFrame) {
	QString message = "Current frame is " + QString::number(currentFrame) + ", visiting main memory address " + QString::number(addr);
	if (isError)
		message += ", page error happend, replace frame " + QString::number(replacedFrame) + " with frame " + QString::number(currentFrame);
	else
		message += ", no page error";
	ui.logList->addItem(message);
	ui.pageErrorNum->setText(QString::number(pageErrorNum));
	ui.pageErrorRate->setText(QString::fromStdString(to_string(pageErrorNum / double(pra->insNum) * 100).substr(0, 4)) + "%");
}

void OS2::toggleAbility(bool isEnable){
	ui.FIFOButton->setEnabled(isEnable);
	ui.LRUButton->setEnabled(isEnable);
	ui.insNum->setEnabled(isEnable);
	ui.frameNum->setEnabled(isEnable);
	ui.pageSize->setEnabled(isEnable);
	ui.singleButton->setEnabled(!isEnable);
	ui.allButton->setEnabled(!isEnable);
}

void OS2::updateInsSeq(){
	ui.insIndex->clear();
	ui.insLogicalAddr->clear();
	for (int i = 0; i < pra->insSeq.size(); i++) {
		ui.insIndex->addItem(QString::number(i));
		ui.insLogicalAddr->addItem(QString::number(pra->insSeq[i]));
	}
}
