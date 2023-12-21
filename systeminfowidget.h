#pragma once

#include <QWidget>
#include "ui_systeminfowidget.h"
#include "SystemInfo.h"
#include "GaugeBoardWidget/gaugeboardwidget.h"
#include <QThread>
class SystemInfoWidget : public QWidget
{
	Q_OBJECT

public:
	enum GaugeBoard
	{
		CPU,
		MEMORY,
		DISK
	};
	SystemInfoWidget(QWidget* parent = nullptr);
	~SystemInfoWidget();
private slots:
	void onRefresh();

signals:
	void requestRefresh();
private:
	QThread* thread = new QThread();
	SystemInfo* systemInfo = new SystemInfo();
	QTimer* monitor_timer = new QTimer(this);
	Ui::SystemInfoWidgetClass ui;
	const QMap<GaugeBoard, QString> gaugeBoardMap{
		{GaugeBoard::CPU, "CPU占用率"},
		{GaugeBoard::MEMORY, "内存占用率"},
		{GaugeBoard::DISK, "磁盘占用率"}
	};
};
