#include "convertbinary.h"

ConvertBinary::ConvertBinary(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConnect();
}
ConvertBinary::~ConvertBinary()
{}

void ConvertBinary::onLineEidtTextChange()
{
	QWidget* widget = (QWidget*)sender();
	for (QLineEdit* lineEdit : this->findChildren<QLineEdit*>())
	{
		disconnect(lineEdit, &QLineEdit::textChanged, this, &ConvertBinary::onLineEidtTextChange);
	}
	if (widget->objectName() == "slideButton")
	{
		ui.SixTeenLineEdit->setText(Tools::convertBase(ui.SixTeenLineEdit->text(), 16, 16, ui.slideButton->isChecked()));
		ui.tenLineEdit->setText(Tools::convertBase(ui.tenLineEdit->text(), 10, 10, ui.slideButton->isChecked()));
		ui.eightLineEdit->setText(Tools::convertBase(ui.eightLineEdit->text(), 8, 8, ui.slideButton->isChecked()));
		ui.twoLineEdit->setText(Tools::convertBase(ui.twoLineEdit->text(), 2, 2, ui.slideButton->isChecked()));
	}
	else
	{
		QLineEdit* lineEdit = (QLineEdit*)widget;
		QString objName = lineEdit->objectName();
		if (objName == "SixTeenLineEdit")
		{
			ui.tenLineEdit->setText(Tools::convertBase(lineEdit->text(), 16, 10, ui.slideButton->isChecked()));
			ui.eightLineEdit->setText(Tools::convertBase(lineEdit->text(), 16, 8, ui.slideButton->isChecked()));
			ui.twoLineEdit->setText(Tools::convertBase(lineEdit->text(), 16, 2, ui.slideButton->isChecked()));
		}
		else if (objName == "tenLineEdit")
		{
			ui.SixTeenLineEdit->setText(Tools::convertBase(lineEdit->text(), 10, 16, ui.slideButton->isChecked()));
			ui.eightLineEdit->setText(Tools::convertBase(lineEdit->text(), 10, 8, ui.slideButton->isChecked()));
			ui.twoLineEdit->setText(Tools::convertBase(lineEdit->text(), 10, 2, ui.slideButton->isChecked()));
		}
		else if (objName == "eightLineEdit")
		{
			ui.SixTeenLineEdit->setText(Tools::convertBase(lineEdit->text(), 8, 16, ui.slideButton->isChecked()));
			ui.tenLineEdit->setText(Tools::convertBase(lineEdit->text(), 8, 10, ui.slideButton->isChecked()));
			ui.twoLineEdit->setText(Tools::convertBase(lineEdit->text(), 8, 2, ui.slideButton->isChecked()));
		}
		else if (objName == "twoLineEdit")
		{
			ui.SixTeenLineEdit->setText(Tools::convertBase(lineEdit->text(), 2, 16, ui.slideButton->isChecked()));
			ui.tenLineEdit->setText(Tools::convertBase(lineEdit->text(), 2, 10, ui.slideButton->isChecked()));
			ui.eightLineEdit->setText(Tools::convertBase(lineEdit->text(), 2, 8, ui.slideButton->isChecked()));
		}
	}

	for (QLineEdit* lineEdit : this->findChildren<QLineEdit*>())
	{
		connect(lineEdit, &QLineEdit::textChanged, this, &ConvertBinary::onLineEidtTextChange);
	}
}
void ConvertBinary::loadConnect()
{
	for (QLineEdit* lineEdit : this->findChildren<QLineEdit*>())
	{
		connect(lineEdit, &QLineEdit::textChanged, this, &ConvertBinary::onLineEidtTextChange);
	}
	connect(ui.slideButton, &SlideButton::clicked, this, &ConvertBinary::onLineEidtTextChange);

	connect(ui.sixtennPushButton, &QPushButton::clicked, [=]() {this->setLineEditText(ui.SixTeenLineEdit, Tools::getClipboard()); });
	connect(ui.tenPushButton, &QPushButton::clicked, [=]() {this->setLineEditText(ui.tenLineEdit, Tools::getClipboard()); });
	connect(ui.eightPushButton, &QPushButton::clicked, [=]() {this->setLineEditText(ui.eightLineEdit, Tools::getClipboard()); });
	connect(ui.twoPushButton, &QPushButton::clicked, [=]() {this->setLineEditText(ui.twoLineEdit, Tools::getClipboard()); });

	connect(ui.copyPushButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.SixTeenLineEdit->text()); });
	connect(ui.copyPushButton_2, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.tenLineEdit->text()); });
	connect(ui.copyPushButton_3, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.eightLineEdit->text()); });
	connect(ui.copyPushButton_4, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.twoLineEdit->text()); });
}

void ConvertBinary::setLineEditText(QLineEdit* lineEidt, QString text)
{
	if (!text.isEmpty())
	{
		lineEidt->setText(text);
	}
}