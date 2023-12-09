#include "uuidgeneration.h"

UuidGeneration::UuidGeneration(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConnect();
}

UuidGeneration::~UuidGeneration()
{}

void UuidGeneration::loadConnect()
{
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.exportsTextEdit->toPlainText()); });
	connect(ui.clearButton, &QPushButton::clicked, ui.exportsTextEdit, &QPlainTextEdit::clear);
	connect(ui.generateButton, &QPushButton::clicked, this, &UuidGeneration::onGenerateButtonClicked);
}

void UuidGeneration::onGenerateButtonClicked()
{
	bool isUpper = ui.upperButton->isChecked();		// 是否大写
	bool isHyphen = ui.hyphenButton->isChecked();	// 是否带横线
	for (int i = 0; i < ui.countspinBox->value(); i++)
	{
		QString uuid = QUuid::createUuid().toString();
		//去除首尾的花括号
		uuid.replace('{', "");
		uuid.replace('}', "");
		if (isUpper)
		{
			uuid = uuid.toUpper();
		}
		else
		{
			uuid = uuid.toLower();
		}
		if (!isHyphen)
		{
			uuid.replace("-", "");
		}
		ui.exportsTextEdit->appendPlainText(uuid);
	}
}
