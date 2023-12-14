#pragma once

#include <QWidget>
#include "ui_settingwidget.h"

class SettingWidget : public QWidget
{
	Q_OBJECT

public:
	SettingWidget(QWidget *parent = nullptr);
	~SettingWidget();

private:
	Ui::SettingWidgetClass ui;
};
