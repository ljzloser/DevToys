#include "settingwidget.h"
#include <qtextedit.h>
#include "sciscintilla.h"
#include <qplaintextedit.h>
SettingWidget::SettingWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConfig();
	this->loadConnect();
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

