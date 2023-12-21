#include "systeminfowidget.h"

SystemInfoWidget::SystemInfoWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	for (int i = 0; i < gaugeBoardMap.keys().count(); ++i)
	{
		GaugeBoard gaugeboard = gaugeBoardMap.keys().at(i);
		GaugeBoardWidget* widget = new GaugeBoardWidget(0.0, 100.0, 2, this);
		QString name = gaugeBoardMap[gaugeboard];
		widget->setName(name);
		widget->setUnit("%");
		ui.SystemInfoLayout->addWidget(widget, i / 3, i % 3);
		switch (gaugeboard)
		{
		case SystemInfoWidget::CPU:
			connect(systemInfo, &SystemInfo::cpuRateChanged, [=](double value) {widget->setValue(value);  });
			break;
		case SystemInfoWidget::MEMORY:
			connect(systemInfo, &SystemInfo::memRateChanged, [=](double value) {widget->setValue(value);  });
			break;
		case SystemInfoWidget::DISK:
			connect(systemInfo, &SystemInfo::diskRateChanged, [=](double value) {widget->setValue(value); });
			break;
		default:
			break;
		}
	}
	systemInfo->moveToThread(thread);
	connect(thread, &QThread::started, systemInfo, &SystemInfo::GetResource);
	connect(this->monitor_timer, &QTimer::timeout, [=]() {emit this->requestRefresh(); });
	connect(this, &SystemInfoWidget::requestRefresh, this, &SystemInfoWidget::onRefresh);
	this->monitor_timer->start(1000);
}

SystemInfoWidget::~SystemInfoWidget()
{
	delete systemInfo;
}

void SystemInfoWidget::onRefresh()
{
	if (thread->isRunning())
	{
		this->thread->exit(0);
	}
	if (!this->isHidden())
	{
		this->thread->start();
	}
}