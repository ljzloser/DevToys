﻿#include "settingwidget.h"
#include <qtextedit.h>
#include "sciscintilla.h"
#include <qplaintextedit.h>
#include <QDesktopServices>
SettingWidget::SettingWidget(QWidget *parent)
	: QWidget(parent)
{
	SqlLog::saveLog("打开设置界面");
	ui.setupUi(this);
	this->loadConfig();
	this->loadConnect();
	ui.tableWidget->setVisible(false);
	ui.beginDateEdit->setDate(QDate::currentDate());
	ui.endDateEdit->setDate(QDate::currentDate().addDays(1));
}

SettingWidget::~SettingWidget()
{}

void SettingWidget::loadConfig()
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
	else
	{
		isOk = false;
	}
	if (isOk)
	{
		Config::setValue(objectName,value);
		loadUi();
	}
}



void SettingWidget::loadConnect()
{
	connect(ui.fontComboBox, &QFontComboBox::currentTextChanged, this, &SettingWidget::saveConfig);
	connect(ui.autoLineButton, &SlideButton::clicked, this, &SettingWidget::saveConfig);
	connect(ui.fontSizespinBox, &QSpinBox::valueChanged, this, &SettingWidget::saveConfig);
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
	// 自适应列宽和行高
	ui.tableWidget->resizeColumnsToContents();
	ui.tableWidget->resizeRowsToContents();

}

void SettingWidget::loadUi(QWidget* widget)
{
	QApplication* app = qobject_cast<QApplication*>(QCoreApplication::instance());

	// 使用应用程序对象获取主窗口指针
	if (app) {
		widget =  widget == nullptr ? qobject_cast<QWidget*>(app->activeWindow()) : widget;
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
			QList<QPlainTextEdit *> plantTextEdits = widget->findChildren<QPlainTextEdit *>();
			for (QPlainTextEdit* plainTextEdit : plantTextEdits)
			{
				plainTextEdit->setFont(font);
				plainTextEdit->setLineWrapMode(ui.autoLineButton->isChecked() ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
			}
			ui.aboutLabel->setFont(font);
			ui.aboutLabel->setWordWrap(ui.autoLineButton->isChecked());


			widget->update();
		}
	}
}

