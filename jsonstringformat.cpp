#include "jsonstringformat.h"

JsonStringFormat::JsonStringFormat(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConnect();
	QsciLexer* imporLexer = new QsciLexerJSON();
	QsciLexer* imporLexer1 = new QsciLexerJSON();
	ui.importWidget->setLexer(imporLexer);
	ui.exportsWidget->setLexer(imporLexer1);
}

JsonStringFormat::~JsonStringFormat()
{}

void JsonStringFormat::loadConnect()
{

	connect(ui.importWidget, &SciScintilla::textChanged, this, &JsonStringFormat::onImportWidgetTextChanged);
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.exportsWidget->text()); });
	connect(ui.ClearButton, &QPushButton::clicked, ui.importWidget, &QsciScintilla::clear);
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.importWidget->setText(Tools::getClipboard()); });
	connect(ui.expandButton, &QPushButton::clicked, [=]() {ui.setGroupBox->setVisible(!ui.setGroupBox->isVisible()); });
	connect(ui.indentComboBox, &QComboBox::currentIndexChanged, this, &JsonStringFormat::onImportWidgetTextChanged);
}


void JsonStringFormat::onImportWidgetTextChanged()
{
	Json::Value json;
	QString text = ui.importWidget->text();
	int indent = 0;

	if (!text.isEmpty())
	{
		Json::Reader reader;
		reader.parse(text.toStdString(), json);
		if (!reader.getFormattedErrorMessages().empty())
		{
			ui.exportsWidget->setText(QString::fromStdString(reader.getFormattedErrorMessages()));
		}
		else
		{
			switch (ui.indentComboBox->currentIndex())
			{
			case 0:
				indent = 4;
				break;
			case 1:
				indent = 2;
				break;
			default:
				indent = 0;
				break;
			}
			ui.exportsWidget->setText(Tools::formatJson(json, indent));
		}
	}
	else
	{
		ui.exportsWidget->clear();
	}
}
