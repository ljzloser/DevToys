#pragma once

#include <QWidget>
#include "ui_settingwidget.h"
#include "Config.h"
#include "sqllog.h"

class SettingWidget : public QWidget
{
	Q_OBJECT

public:
	SettingWidget(QWidget* parent = nullptr);
	~SettingWidget();
signals:
	void themeChanged(int);
	void borderColorChanged(QColor);
	void borderRadiusChanged(int);
	void borderSizeChanged(int);
	void opacityChanged(int);
public slots:
	void loadUi(QWidget* widget = nullptr);
private:
	Ui::SettingWidgetClass ui;
	void loadConfig() const;
	void loadConnect();
	void loadLog();
	void showEvent(QShowEvent* event) override;
	int theme = -1;
private slots:
	void saveConfig();
};
