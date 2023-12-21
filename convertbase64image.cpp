#include "convertbase64image.h"
#include <QBuffer>
#include <QFileDialog>
#include <qmessagebox.h>

ConvertBase64Image::ConvertBase64Image(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConnect();
	// 开启拖入
	this->setAcceptDrops(true);
}

ConvertBase64Image::~ConvertBase64Image()
{}

void ConvertBase64Image::onPlainTextEditChanged()
{
	QString text = ui.plainTextEdit->toPlainText();
	if (text.isEmpty())
	{
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	QStringList list = text.split(',');
	if (list.size() != 2)
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	QStringList imageInfoList = list[0].split(';');
	if (imageInfoList.size() != 2)
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	QStringList typeList = imageInfoList[0].split(':');//data:image/png;base64
	if (typeList.size() != 2)
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	if (typeList[0] != "data")
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	QStringList dataList = typeList[1].split('/');
	if (dataList.size() != 2)
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	QString data = dataList[0];//image
	QString type = dataList[1];//png
	if (data != "image")
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}

	if (!typeLists.contains(type.toLower()))
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	if (imageInfoList[1] != "base64")
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	this->imageType = type;

	// 尝试转换为Base64
	QByteArray base64 = QByteArray::fromBase64(list[1].toUtf8());
	QImage image;
	image.loadFromData(base64);
	if (image.isNull())
	{
		Tools::showPopupMessage("解析图片格式信息失败");
		this->pixmap = QPixmap();
		this->imageType = "";
		ui.imageLabel->clear();
		return;
	}
	this->pixmap = QPixmap::fromImage(image);
	QPixmap newPixmap = this->pixmap.scaled(ui.imageLabel->width(), ui.imageLabel->height(), Qt::KeepAspectRatio);
	ui.imageLabel->setPixmap(newPixmap);
}

void ConvertBase64Image::loadConnect()
{
	connect(ui.clearButton, &QPushButton::clicked, ui.plainTextEdit, &QPlainTextEdit::clear);
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.plainTextEdit->toPlainText()); });
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.plainTextEdit->setPlainText(Tools::getClipboard()); });
	connect(ui.openFileButton, &QPushButton::clicked, [=]() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), ".");
		if (!fileName.isEmpty())
		{
			QFile file(fileName);
			if (file.open(QIODevice::ReadOnly))
			{
				ui.plainTextEdit->setPlainText(file.readAll());
			}
			else
			{
				QMessageBox::warning(this, tr("警告"), file.errorString());
			}
		}
		});
	connect(ui.plainTextEdit, &QPlainTextEdit::textChanged, this, &ConvertBase64Image::onPlainTextEditChanged);
	connect(ui.openImageButton, &QPushButton::clicked, [=]()
		{
			//"png", "jpeg", "jpg", "bmp", "gif","ico","webp","svg" 可选择的 图片格式
			QString fileName = QFileDialog::getOpenFileName(this, tr("图片文件"), ".", "image(*.png *.jpeg *.jpg *.bmp *.gif *.ico *.webp *.svg)");
			if (!fileName.isEmpty())
			{
				this->onLoadImage(fileName);
			}
		});
	connect(ui.saveImageButton, &QPushButton::clicked, this, &ConvertBase64Image::onSaveImage);
	connect(ui.copyImageButton, &QPushButton::clicked, this, [=]()
		{
			if (!this->pixmap.isNull())
			{
				Tools::setClipboardPixmap(this->pixmap);
			}
			else
			{
				Tools::showPopupMessage("图片为空");
			}
		});
	connect(ui.pasteImageButton, &QPushButton::clicked, this, [=]()
		{
			QPixmap pixmap = Tools::getClipboardPixmap();
			if (!pixmap.isNull())
			{
				this->pixmap = pixmap;
				this->imageType = "png";
				QPixmap newPixmap = this->pixmap.scaled(ui.imageLabel->width(), ui.imageLabel->height(), Qt::KeepAspectRatio);
				QImage image = pixmap.toImage();
				QByteArray byteArray;
				QBuffer buffer(&byteArray);
				image.save(&buffer, this->imageType.toUtf8().constData());
				//获取后缀
				if (this->imageType.toLower() == "svg")
				{
					this->imageType = "svg+xml";
				}
				else if (this->imageType.toLower() == "jpg")
				{
					this->imageType = "jpeg";
				}
				else if (this->imageType.toLower() == "ico")
				{
					this->imageType = "x-icon";
				}
				this->pixmap = QPixmap::fromImage(image);
				ui.plainTextEdit->setPlainText(QString("data:image/%1;base64,%2").arg(this->imageType).arg(byteArray.toBase64()));
			}
		});
	connect(ui.seeImageButton, &QPushButton::clicked, [=]()
		{
			if (this->pixmap.isNull())
			{
				Tools::showPopupMessage("图片为空");
				return;
			}
			//随机生成一个临时文件名
			QString tempFileName = QApplication::applicationDirPath() + "\\temp\\" + QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) + ".png";
			// 判断有没有temp文件夹
			if (!QDir(QApplication::applicationDirPath() + "\\temp").exists())
			{
				// 没有就创建
				QDir().mkdir(QApplication::applicationDirPath() + "\\temp");
			}
			this->pixmap.save(tempFileName, "PNG"); // 保存为 PNG 格式
			QDesktopServices::openUrl(QUrl::fromLocalFile(tempFileName));
		});
}

void ConvertBase64Image::resizeEvent(QResizeEvent* event)
{
	if (this->pixmap.isNull())
	{
		return;
	}
	ui.imageLabel->clear();
	ui.scrollAreaWidgetContents->setFixedWidth(ui.scrollArea->width());
	QRect rect = ui.imageLabel->geometry();
	rect.setBottom(ui.plainTextEdit->geometry().bottom());
	rect.setRight(ui.frame_3->geometry().right());
	QPixmap newPixmap = this->pixmap.scaled(ui.imageLabel->width(), ui.imageLabel->height(), Qt::KeepAspectRatio);
	ui.imageLabel->setPixmap(newPixmap);
}

void ConvertBase64Image::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept();
	if (event->mimeData()->hasUrls())
	{
		QString fileName = event->mimeData()->urls()[0].toLocalFile();
		QString suffix = QFileInfo(fileName).suffix().toLower();
		QSet<QString> validImageFormats{ "png", "jpeg", "jpg", "bmp", "gif", "ico", "webp", "svg" };

		if (validImageFormats.contains(suffix)) {
			event->acceptProposedAction();
		}
		else {
			event->ignore();
		}
	}
}

void ConvertBase64Image::dropEvent(QDropEvent* event)
{
	// 处理拖放数据
	const QMimeData* mimeData = event->mimeData();
	QString fileName = event->mimeData()->urls()[0].toLocalFile();
	onLoadImage(fileName);
}

void ConvertBase64Image::dragMoveEvent(QDragMoveEvent* event)
{
	if (!isWithinDropArea())
	{
		event->ignore();
		return;
	}
	else
	{
		event->acceptProposedAction();
	}
}

void ConvertBase64Image::onSaveImage()
{
	if (this->pixmap.isNull())
	{
		Tools::showPopupMessage("没有图片无法保存！");
		return;
	}
	QString type = this->imageType;
	if (type.isEmpty())
	{
		return;
	}
	if (type == "svg+xml")
	{
		type = "svg";
	}
	else if (type == "x-icon")
	{
		type = "ico";
	}
	QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), ".", type + "(*." + type + ")");
	if (!fileName.isEmpty())
	{
		if (this->pixmap.save(fileName, type.toUtf8().constData()))
		{
			QMessageBox::information(this, tr("提示"), tr("保存成功"));
		}
		else
		{
			QMessageBox::warning(this, tr("警告"), tr("保存失败"));
		}
	}
}

void ConvertBase64Image::onLoadImage(QString fileName)
{
	QImageReader imageReader(fileName);
	if (!fileName.isEmpty())
	{
		this->imageType = QString::fromUtf8(imageReader.format());
		if (this->imageType.isEmpty())
		{
			Tools::showPopupMessage(imageReader.errorString());
			return;
		}
		QImage image = imageReader.read();
		if (image.isNull())
		{
			Tools::showPopupMessage(imageReader.errorString());
			return;
		}
		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		image.save(&buffer, this->imageType.toUtf8().constData());
		//获取后缀
		if (this->imageType.toLower() == "svg")
		{
			this->imageType = "svg+xml";
		}
		else if (this->imageType.toLower() == "jpg")
		{
			this->imageType = "jpeg";
		}
		else if (this->imageType.toLower() == "ico")
		{
			this->imageType = "x-icon";
		}
		this->pixmap = QPixmap::fromImage(image);
		ui.plainTextEdit->setPlainText(QString("data:image/%1;base64,%2").arg(this->imageType).arg(byteArray.toBase64()));
	}
}