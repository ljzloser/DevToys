#include "xmlstringformat.h"
#include <QFileDialog>
#include <qmessagebox.h>

XmlStringFormat::XmlStringFormat(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConnect();
	QsciLexer* imporLexer = new QsciLexerXML();
	QsciLexer* imporLexer1 = new QsciLexerXML();
	ui.importWidget->setLexer(imporLexer);
	ui.exportsWidget->setLexer(imporLexer1);
}

XmlStringFormat::~XmlStringFormat()
{}
void XmlStringFormat::loadConnect()
{

	connect(ui.importWidget, &SciScintilla::textChanged, this, &XmlStringFormat::onImportWidgetTextChanged);
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.exportsWidget->text()); });
	connect(ui.ClearButton, &QPushButton::clicked, ui.importWidget, &QsciScintilla::clear);
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.importWidget->setText(Tools::getClipboard()); });
	connect(ui.expandButton, &QPushButton::clicked, [=]() {ui.setGroupBox->setVisible(!ui.setGroupBox->isVisible()); });
	connect(ui.indentComboBox, &QComboBox::currentIndexChanged, this, &XmlStringFormat::onImportWidgetTextChanged);
	connect(ui.OpenFileButton, &QPushButton::clicked, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), ".");
			if (!fileName.isEmpty())
			{
				QFile file(fileName);
				if (file.open(QIODevice::ReadOnly))
				{
					ui.importWidget->setText(file.readAll());
				}
				else
				{
					QMessageBox::warning(this, tr("警告"), file.errorString());
				}
			}
		});
}


void XmlStringFormat::onImportWidgetTextChanged()
{
	QString errorString = "";
	QString text = ui.importWidget->text();
	QString formatString = "";
	int indent = 0;
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
	if (!text.isEmpty())
	{
		if (Tools::formatXml(text,indent, formatString,errorString))
		{
			ui.exportsWidget->setText(formatString);
		}
		else
		{

			ui.exportsWidget->setText(errorString);
		}
	}
	else
	{
		ui.exportsWidget->clear();
	}
}
