#include "settingwidget.h"
#include <qtextedit.h>
#include "sciscintilla.h"
#include <qplaintextedit.h>
#include <QDesktopServices>
#include <qdebug.h>
#include "tools.h"
#include <qcolordialog.h>
SettingWidget::SettingWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConfig();
	this->loadConnect();
	ui.tableWidget->setVisible(false);
	ui.beginDateEdit->setDate(QDate::currentDate());
	ui.endDateEdit->setDate(QDate::currentDate().addDays(1));
	ui.textBrowser->setMarkdown(
	R"(
# QDevToys

#### 介绍
- 无聊写的仿Devtoys工具，说实话Devtoys有好多bug尤其是那个复制，我只能点按钮，快捷键直接崩溃,再加上一点点吐槽。

#### 软件架构
- 本程序基于VS2022+Qt6.3.0,并使用了以下第三方库,图片资源来自阿里巴巴矢量图标库.
	- 1.QsciScintilla;
	- 2.Json-cpp;
	- 3.YamI-cpp:
	- 4.qdarkstyle;
)");
	ui.frame_8->setVisible(false);
	ui.frame_9->setVisible(false);
	ui.frame_10->setVisible(false);
	ui.frame_11->setVisible(false);
	ui.frame_2->setVisible(false);
	ui.frame_3->setVisible(false);
	ui.plainTextEdit->setVisible(false);
	ui.frame_6->setVisible(false);
	ui.textBrowser->setVisible(false);
}

SettingWidget::~SettingWidget()
{
	SqlLog::writeLog();
}

void SettingWidget::loadConfig() const
{
	QJsonObject config = Config::getConfig();
	for (auto it = config.begin(); it != config.end(); ++it)
	{
		QString objectName = it.key();
		QVariant value = it.value().toVariant();
		QObject* obj = this->findChild<QObject*>(objectName);
		if (obj)
		{
			if (qobject_cast<QComboBox*>(obj))
			{
				QComboBox* comboBox = qobject_cast<QComboBox*>(obj);
				comboBox->setCurrentText(value.toString());
			}
			else if (qobject_cast<SlideButton*>(obj))
			{
				SlideButton* slideButton = qobject_cast<SlideButton*>(obj);
				slideButton->setDefaultChecked(value.toBool());
			}
			else if (qobject_cast<QSpinBox*>(obj))
			{
				QSpinBox* spinBox = qobject_cast<QSpinBox*>(obj);
				spinBox->setValue(value.toInt());
			}
			else if (qobject_cast<QPushButton*>(obj))
			{
				QPushButton* pushButton = qobject_cast<QPushButton*>(obj);
				if (objectName == "colorButton")
				{
					QColor color = value.value<QColor>();
					pushButton->setStyleSheet(QString("background-color:%1;").arg(color.name()));
				}
			}
			else if (qobject_cast<QPlainTextEdit*>(obj))
			{
				QPlainTextEdit* plainTextEdit = qobject_cast<QPlainTextEdit*>(obj);
				plainTextEdit->setPlainText(value.toString());
			}
			else
			{

			}
		}
	}
}
void SettingWidget::saveConfig()
{
	if (!sender()) return;
	QString objectName = sender()->objectName();
	QVariant value;
	bool isOk = true;
	if (qobject_cast<QComboBox*>(sender()))
	{
		QComboBox* comboBox = qobject_cast<QComboBox*>(sender());
		value = comboBox->currentText();
	}
	else if (qobject_cast<SlideButton*>(sender()))
	{
		SlideButton* slideButton = qobject_cast<SlideButton*>(sender());
		value = slideButton->isChecked();
	}
	else if (qobject_cast<QSpinBox*>(sender()))
	{
		QSpinBox* spinBox = qobject_cast<QSpinBox*>(sender());
		value = spinBox->value();
	}
	else if (qobject_cast<QPlainTextEdit*>(sender()))
	{
		QPlainTextEdit* plainTextEdit = qobject_cast<QPlainTextEdit*>(sender());
		value = plainTextEdit->toPlainText();
	}
	else
	{
		isOk = false;
	}
	if (isOk)
	{
		Config::setValue(objectName, value);
		loadUi();
	}
}



void SettingWidget::loadConnect()
{
	connect(ui.fontComboBox, &QFontComboBox::currentTextChanged, this, &SettingWidget::saveConfig);
	connect(ui.autoLineButton, &SlideButton::clicked, this, &SettingWidget::saveConfig);
	connect(ui.fontSizespinBox, &QSpinBox::valueChanged, this, &SettingWidget::saveConfig);
	connect(ui.themeComboBox, &QComboBox::currentTextChanged, this, &SettingWidget::saveConfig);
	connect(ui.openLineButton, &QPushButton::clicked, [=]()
		{
			QUrl url("https://github.com/ljzloser/DevToys");
			QDesktopServices::openUrl(url);
		});
	connect(ui.openLineButton_2, &QPushButton::clicked, [=]()
		{
			QUrl url("http://saolei.wang/Player/Index.asp?Id=21720");
			QDesktopServices::openUrl(url);
		});
	connect(ui.pushButton, &QPushButton::clicked, [=]()
		{
			ui.tableWidget->setVisible(!ui.tableWidget->isVisible());
		});
	connect(ui.beginDateEdit, &QDateEdit::dateChanged, this, &SettingWidget::loadLog);
	connect(ui.endDateEdit, &QDateEdit::dateChanged, this, &SettingWidget::loadLog);
	connect(ui.colorButton, &QPushButton::clicked, [=]()
		{
			QColorDialog dialog(ui.colorButton->palette().color(QPalette::Button),this);
			if (dialog.exec())
			{
				ui.colorButton->setStyleSheet(QString("background-color:%1").arg(dialog.currentColor().name()));
				emit borderColorChanged(ui.colorButton->palette().color(QPalette::Button).name());
				Config::setValue("colorButton", dialog.currentColor().name());
			}
		});
	connect(ui.boderRadiusSpinBox, &QSpinBox::valueChanged, this, &SettingWidget::saveConfig);
	connect(ui.boderSizeSpinBox, &QSpinBox::valueChanged, this, &SettingWidget::saveConfig);
	connect(ui.plainTextEdit, &QPlainTextEdit::textChanged, this, &SettingWidget::saveConfig);
	connect(ui.opacitySpinBox, &QSpinBox::valueChanged, this, &SettingWidget::saveConfig);
	connect(ui.expandButton, &QPushButton::clicked, [=]()
		{
			ui.frame_8->setVisible(!ui.frame_8->isVisible());
			ui.frame_9->setVisible(!ui.frame_9->isVisible());
			ui.frame_10->setVisible(!ui.frame_10->isVisible());
			ui.frame_11->setVisible(!ui.frame_11->isVisible());
		});
	connect(ui.expandButton_2, &QPushButton::clicked, [=]()
		{
			ui.frame_2->setVisible(!ui.frame_2->isVisible());
			ui.frame_3->setVisible(!ui.frame_3->isVisible());
			ui.plainTextEdit->setVisible(!ui.plainTextEdit->isVisible());
		});
	connect(ui.expandButton_3, &QPushButton::clicked, [=]()
		{
			ui.frame_6->setVisible(!ui.frame_6->isVisible());
			ui.textBrowser->setVisible(!ui.textBrowser->isVisible());
		});
}

void SettingWidget::loadLog()
{
	QList<QVariantMap> result = SqlLog::readLog(ui.beginDateEdit->date(), ui.endDateEdit->date());
	ui.tableWidget->clear();
	if (result.size() > 0)
	{
		// 创建表头
		QStringList header;
		QVariantMap map = result[0];
		header << "序号" << "时间" << "日志";
		ui.tableWidget->setColumnCount(header.size());
		ui.tableWidget->setHorizontalHeaderLabels(header);
		// 添加数据
		ui.tableWidget->setRowCount(result.size());
		for (int i = 0; i < result.size(); ++i)
		{
			QVariantMap map = result[i];
			for (QString key : header)
			{
				ui.tableWidget->setItem(i, header.indexOf(key), new QTableWidgetItem(map.value(key).toString()));
			}
		}
	}
	else
	{
		ui.tableWidget->setRowCount(0);
		ui.tableWidget->setColumnCount(0);
	}
	//// 自适应列宽和行高
	//ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->resizeRowsToContents();

}

void SettingWidget::showEvent(QShowEvent* event)
{
	SqlLog::writeLog();
	this->loadLog();
}

void SettingWidget::loadUi(QWidget* widget)
{
	QApplication* app = qobject_cast<QApplication*>(QCoreApplication::instance());

	// 使用应用程序对象获取主窗口指针
	if (app) {
		widget = widget == nullptr ? qobject_cast<QWidget*>(app->activeWindow()) : widget;
		if (widget) {
			QFont font = ui.fontComboBox->currentFont();
			font.setPointSize(ui.fontSizespinBox->value());
			QList<SciScintilla*> scintillas = widget->findChildren<SciScintilla*>();
			for (SciScintilla* scintilla : scintillas)
			{
				scintilla->setFont(font);
				scintilla->lexer()->setDefaultFont(font);
				scintilla->lexer()->setFont(font);
				scintilla->setWrapMode(ui.autoLineButton->isChecked() ? QsciScintilla::WrapMode::WrapWord : QsciScintilla::WrapMode::WrapNone);
			}
			QList<QPlainTextEdit*> plantTextEdits = widget->findChildren<QPlainTextEdit*>();
			for (QPlainTextEdit* plainTextEdit : plantTextEdits)
			{
				plainTextEdit->setFont(font);
				plainTextEdit->setLineWrapMode(ui.autoLineButton->isChecked() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
			}
			//ui.aboutLabel->setFont(font);
			//ui.aboutLabel->setWordWrap(ui.autoLineButton->isChecked());

			bool isdark = false;
			switch (ui.themeComboBox->currentIndex())
			{
			case 0:
			{
				isdark = false;
				break;
			}
			case 1:
			{
				isdark = true;
				break;
			}
			case 2:
			{
				isdark = Tools::IsSystemDarkModeActive();
				break;
			}
			default:
				break;
			}
			if ( (isdark != (theme == 1)) || theme == -1)
			{
				QString filename = isdark ? ":/qdarkstyle/dark/darkstyle.qss" : ":/qdarkstyle/light/lightstyle.qss";
				QFile file(filename);
				file.open(QFile::ReadOnly);
				if (file.isOpen())
				{
					QString styleSheet = QLatin1String(file.readAll());
					app->setStyleSheet(styleSheet);
				}
				theme = isdark;
				emit themeChanged(theme);
			}
			emit borderColorChanged(ui.colorButton->palette().color(QPalette::Button).name());
			emit borderRadiusChanged(ui.boderRadiusSpinBox->value());
			emit borderSizeChanged(ui.boderSizeSpinBox->value());
			emit this->opacityChanged(ui.opacitySpinBox->value());
			widget->update();
		}
	}
}

