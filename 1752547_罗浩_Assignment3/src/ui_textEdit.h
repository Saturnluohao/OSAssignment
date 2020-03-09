#ifndef UI_TEXTEDIT_H
#define UI_TEXTEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include <QMessageBox>
#include "myFS2.h"

QT_BEGIN_NAMESPACE

class textEditer : public QWidget
{
	Q_OBJECT
public:
	textEditer(const char* title, myFile* _file) {
		setupUi(this, title);
		file = _file;
		file->read(chars);
		auto content = QString::fromStdString(string(chars));
		textEdit->setText(content);
		QObject::connect(saveButton, &QPushButton::clicked, this, &textEditer::saveFile);
	}

	textEditer() {
		if (chars != NULL)
			delete[] chars;
	}

	QTextEdit *textEdit;
	QPushButton *saveButton;
	myFile* file;

	void setupUi(QWidget *Form, const char* title)
	{
		if (Form->objectName().isEmpty())
			Form->setObjectName(QString::fromUtf8("Form"));
		Form->resize(403, 304);
		textEdit = new QTextEdit(Form);
		textEdit->setObjectName(QString::fromUtf8("textEdit"));
		textEdit->setGeometry(QRect(0, 10, 401, 251));
		QFont font;
		font.setFamily(QString::fromUtf8("Times New Roman"));
		font.setPointSize(12);
		textEdit->setFont(font);
		saveButton = new QPushButton(Form);
		saveButton->setObjectName(QString::fromUtf8("saveButton"));
		saveButton->setGeometry(QRect(150, 270, 75, 23));
		saveButton->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 255, 0);"));

		retranslateUi(Form, title);

		QMetaObject::connectSlotsByName(Form);
	} // setupUi

	void retranslateUi(QWidget *Form, const char* title)
	{
		Form->setWindowTitle(QApplication::translate("Form", title, nullptr));
		saveButton->setText(QApplication::translate("Form", "\344\277\235\345\255\230", nullptr));
	} // retranslateUi

	void saveFile() {
		auto content = textEdit->toPlainText();
		file->write(content.toStdString());
		QMessageBox::information(NULL, "Indication", "File saved successfully!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}

private:
	char* chars;
};

QT_END_NAMESPACE

#endif // UI_TEXTEDIT_H