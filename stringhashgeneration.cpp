#include "stringhashgeneration.h"
#include <QFileDialog>
#include <qmessagebox.h>

StringHashGeneration::StringHashGeneration(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConnect();
}

StringHashGeneration::~StringHashGeneration()
{}

void StringHashGeneration::loadConnect()
{
	connect(ui.MD5CopyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.MD5LineEdit->text()); });
	connect(ui.SHA1CopyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.SHA1LineEdit->text()); });
	connect(ui.SHA256CopyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.SHA256LineEdit->text()); });
	connect(ui.SHA512CopyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.SHA512LineEdit->text()); });
	connect(ui.clearButton, &QPushButton::clicked, ui.importTextEdit, &QPlainTextEdit::clear);
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.importTextEdit->setPlainText(Tools::getClipboard()); });
	connect(ui.openFileButton, &QPushButton::clicked, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), ".");
			if (!fileName.isEmpty())
			{
				QFile file(fileName);
				if (file.open(QIODevice::ReadOnly))
				{
					ui.importTextEdit->setPlainText(file.readAll());
				}
				else
				{
					QMessageBox::warning(this, tr("警告"), file.errorString());
				}
			}
		});
	connect(ui.lowerButton, &SlideButton::clicked, this, &StringHashGeneration::onImportTextEditChanged);
	connect(ui.comboBox, &QComboBox::currentIndexChanged, this, &StringHashGeneration::onImportTextEditChanged);
	connect(ui.importTextEdit, &QPlainTextEdit::textChanged, this, &StringHashGeneration::onImportTextEditChanged);
}

void StringHashGeneration::onImportTextEditChanged()
{
	if (ui.importTextEdit->toPlainText().isEmpty())
	{
		ui.MD5LineEdit->clear();
		ui.SHA1LineEdit->clear();
		ui.SHA256LineEdit->clear();
		ui.SHA512LineEdit->clear();
		return;
	}
	bool isUpper = ui.lowerButton->isChecked();
	int type = ui.comboBox->currentIndex();
	QString text = ui.importTextEdit->toPlainText();
	QByteArray MD5 = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Md5);
	QByteArray SHA1 = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha1);
	QByteArray SHA256 = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha256);
	QByteArray SHA512 = QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha512);
	if (type == 1)
	{
		ui.MD5LineEdit->setText(MD5.toBase64());
		ui.SHA1LineEdit->setText(SHA1.toBase64());
		ui.SHA256LineEdit->setText(SHA256.toBase64());
		ui.SHA512LineEdit->setText(SHA512.toBase64());
	}
	else
	{
		if (isUpper)
		{
			ui.MD5LineEdit->setText(MD5.toHex().toUpper());
			ui.SHA1LineEdit->setText(SHA1.toHex().toUpper());
			ui.SHA256LineEdit->setText(SHA256.toHex().toUpper());
			ui.SHA512LineEdit->setText(SHA512.toHex().toUpper());
		}
		else
		{
			ui.MD5LineEdit->setText(MD5.toHex().toLower());
			ui.SHA1LineEdit->setText(SHA1.toHex().toLower());
			ui.SHA256LineEdit->setText(SHA256.toHex().toLower());
			ui.SHA512LineEdit->setText(SHA512.toHex().toLower());
		}
	}
}