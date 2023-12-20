#include "converthtml.h"
#include <QFileDialog>
#include <qmessagebox.h>

ConvertHtml::ConvertHtml(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.slideButton->setName({ "解码","编码" });
	this->loadConnect();
}

ConvertHtml::~ConvertHtml()
{}

void ConvertHtml::loadConnect()
{
	connect(ui.ClearButton, &QPushButton::clicked, ui.importPlainTextEdit, &QPlainTextEdit::clear);
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.importPlainTextEdit->setPlainText(Tools::getClipboard()); });
	connect(ui.OpenFileButton, &QPushButton::clicked, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), ".");
			if (!fileName.isEmpty())
			{
				QFile file(fileName);
				if (file.open(QIODevice::ReadOnly))
				{
					ui.importPlainTextEdit->setPlainText(file.readAll());
				}
				else
				{
					QMessageBox::warning(this, tr("警告"), file.errorString());
				}
			}
		});
	connect(ui.expandButton, &QPushButton::clicked, [=]()
		{
			ui.groupBox->setVisible(!ui.groupBox->isVisible());
			ui.widget_3->setVisible(!ui.widget_3->isVisible());
		});
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.exportsPlainTextEdit->toPlainText()); });
	connect(ui.importPlainTextEdit, &QPlainTextEdit::textChanged, [=]()
		{
			QString text = ui.importPlainTextEdit->toPlainText();
			if (ui.slideButton->isChecked())
			{
				ui.exportsPlainTextEdit->setPlainText(Tools::htmlEncode(text));
			}
			else
			{
				ui.exportsPlainTextEdit->setPlainText(Tools::htmlDecode(text));
			}
		});
	connect(ui.slideButton, &SlideButton::clicked, [=]()
		{
			if (!ui.slideButton->isChecked())
			{
				ui.importPlainTextEdit->setPlainText(Tools::htmlEncode(ui.importPlainTextEdit->toPlainText()));
			}
			else
			{
				ui.importPlainTextEdit->setPlainText(Tools::htmlDecode(ui.importPlainTextEdit->toPlainText()));
			}
		});
}