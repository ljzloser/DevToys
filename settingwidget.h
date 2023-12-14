#pragma once

#include <QWidget>
#include "ui_settingwidget.h"
#include "Config.h"
class SettingWidget : public QWidget
{
	Q_OBJECT

public:
	SettingWidget(QWidget *parent = nullptr);
	~SettingWidget();
	void loadUi(QWidget * widget = nullptr);
private:
	Ui::SettingWidgetClass ui;
	void loadConfig();
	void loadConnect();

private slots:
	void saveConfig();

};
