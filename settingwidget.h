#pragma once

#include <QWidget>
#include "ui_settingwidget.h"
#include "Config.h"
#include "sqllog.h"
class SettingWidget : public QWidget
{
	Q_OBJECT

public:
	SettingWidget(QWidget *parent = nullptr);
	~SettingWidget();
signals:
	void themeChanged(int);
	void borderColorChanged(QColor);
	void borderRadiusChanged(int);
public slots:
	void loadUi(QWidget * widget = nullptr);
private:
	Ui::SettingWidgetClass ui;
	void loadConfig();
	void loadConnect();
	void loadLog();
	void showEvent(QShowEvent* event) override;
	int theme = -1;
private slots:
	void saveConfig();

};
