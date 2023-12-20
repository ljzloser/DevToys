#include "escapestringwidget.h"

EscapeStringWidget::EscapeStringWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.slideButton->setName({ "反转义","转义" });
	this->loadConnect();
}

EscapeStringWidget::~EscapeStringWidget()
{}

void EscapeStringWidget::loadConnect()
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
			ui.exportsPlainTextEdit->setPlainText(Tools::escapeString(ui.importPlainTextEdit->toPlainText(), ui.slideButton->isChecked()));
		});
	connect(ui.slideButton, &SlideButton::clicked, [=]()
		{
			ui.importPlainTextEdit->setPlainText(Tools::escapeString(ui.importPlainTextEdit->toPlainText(), !ui.slideButton->isChecked()));
		});
}