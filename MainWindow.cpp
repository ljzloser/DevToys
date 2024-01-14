#include "MainWindow.h"
#include <numeric>
#include <qmainwindow.h>
#include "sqllog.h"
#include "windows.h"
#include "windowsx.h"
#include <qmath.h>

#pragma comment (lib,"user32.lib")

MainWindow::MainWindow(QWidget* parent) : QCustomMainWindow(parent)
{
	SqlLog::saveLog("QDevToys启动");

	this->loadConnect();
	this->loadUi();
	QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
	animation->setDuration(500);
	animation->setStartValue(0.0);
	QVariant opacity = Config::getValue("opacitySpinBox");
	animation->setEndValue(opacity.isValid() ? opacity.toDouble() / 100 : 1.0);
	animation->start(QPropertyAnimation::DeleteWhenStopped);
	devtoys->settingWidget->loadUi(this);
}

void MainWindow::loadUi()
{
	titleBar->setTitleText("QDevToys");
	this->setTitleBar(titleBar);
	this->setMainWidget(devtoys);
	this->setBorderColor(Qt::GlobalColor::lightGray);
	this->setEdgeSize(8);
	this->setWindowOpacity(0.0);
}

void MainWindow::loadConnect()
{
	QObject::connect(titleBar, &QCustomTitleBar::closeButtonClicked, this, &MainWindow::close);
	//QObject::connect(titleBar, &QCustomTitleBar::maxButtonClicked, this, &MainWindow::showCustomNormal);
	QObject::connect(titleBar, &QCustomTitleBar::minButtonClicked, this, &MainWindow::showMinimized);
	QObject::connect(this, &MainWindow::windowStateChanged, titleBar, &QCustomTitleBar::updateIcon);
	QObject::connect(titleBar->comboBox, &QComboBox::currentTextChanged, devtoys, &DevToys::onFiterComboBoxTextChanged);
	QObject::connect(titleBar->topButton, &QPushButton::clicked, [=](bool checked)
		{
			// 切换窗口的置顶状态
			if (windowFlags() & Qt::WindowStaysOnTopHint) {
				// 当前是置顶，移除置顶标志
				setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
				titleBar->topButton->setIcon(QIcon(":/DevToys/icon/top.png"));
				QRect rect = QGuiApplication::primaryScreen()->geometry();
				QRect newRect(0, 0, 1280, 720);
				newRect.moveCenter(rect.center());
				this->setGeometry(newRect);
				devtoys->splitter->setSizes({ 350,1280 - 350 });
				titleBar->topButton->setToolTip("置顶");
			}
			else {
				// 当前不是置顶，添加置顶标志
				setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
				titleBar->topButton->setIcon(QIcon(":/DevToys/icon/down.png"));
				QRect rect = QGuiApplication::primaryScreen()->geometry();
				QRect newRect(0, 0, 720, 720);
				newRect.moveCenter(rect.center());
				this->setGeometry(newRect);
				QList<int> sizes = devtoys->splitter->sizes();
				devtoys->splitter->setSizes({ 0, 720 });
				titleBar->topButton->setToolTip("取消置顶");
			}
#ifdef Q_OS_WIN
			HWND hwnd = (HWND)this->winId();
			DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
			::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
#endif
			// 重新显示窗口以使更改生效
			show();
		});
	QObject::connect(this, &MainWindow::systemSettingsChanged, [=]()
		{
			devtoys->settingWidget->loadUi(this);
		});
	QObject::connect(devtoys->settingWidget, &SettingWidget::themeChanged, [=](int theme)
		{
			if (theme == 0)
			{
				//背景色为#19232D
				this->setBackgroundColor(QColor("#fafafa"));
				//分割线颜色
				this->setSplitLineColor(QColor("#fafafa"));
				titleBar->setTitleIcon(QIcon(":/DevToys/icon/生成类型工具-black.png"));
			}
			else
			{
				//背景色为#19232D
				this->setBackgroundColor(QColor("#19232D"));
				//分割线颜色
				this->setSplitLineColor(QColor("#19232D"));
				titleBar->setTitleIcon(QIcon(":/DevToys/icon/生成类型工具.png"));
			}
		});
	QObject::connect(devtoys->settingWidget, &SettingWidget::borderColorChanged, [=](QColor color)
		{
			this->setBorderColor(color);
			this->update();
		});
	QObject::connect(devtoys->settingWidget, &SettingWidget::borderRadiusChanged, [=](int radius) {this->setRadius(radius); });
	QObject::connect(devtoys->settingWidget, &SettingWidget::borderSizeChanged, [=](int size) {this->setBorderSize(size); });
	QObject::connect(devtoys->settingWidget, &SettingWidget::opacityChanged, [=](int size) {this->setWindowOpacity(size / 100.0); });
}

MainWindow::~MainWindow()
{
	SqlLog::saveLog("QDevToys退出", true);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier) && event->key() == Qt::Key_F)
	{
		titleBar->comboBox->setFocus();
	}
}