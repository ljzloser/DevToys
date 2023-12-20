#include "markdownpreviewwidget.h"
#include "tools.h"
#include <qfile.h>
#include <qfiledialog.h>
#include <qmessagebox.h>

MarkDownPreviewWidget::MarkDownPreviewWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.importWidget->setLexer(new QsciLexerMarkdown());
	ui.importWidget->setIndentationsUseTabs(true); // 使用制表符
	this->loadConnect();
}

MarkDownPreviewWidget::~MarkDownPreviewWidget()
{}

void MarkDownPreviewWidget::loadConnect()
{
	connect(ui.importWidget, &SciScintilla::textChanged, this, [=]()
		{
			ui.textBrowser->setMarkdown(ui.importWidget->text());
		});
	connect(ui.ClearButton, &QPushButton::clicked, ui.importWidget, &SciScintilla::clear);
	connect(ui.ClearButton, &QPushButton::clicked, ui.textBrowser, &QTextBrowser::clear);
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.textBrowser->toHtml()); });
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.importWidget->setText(Tools::getClipboard()); });
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