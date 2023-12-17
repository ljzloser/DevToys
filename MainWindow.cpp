#include "MainWindow.h"

#include <qmainwindow.h>
#include "sqllog.h"
#include "windows.h"
#include "windowsx.h"
#pragma comment (lib,"user32.lib")

MainWindow::MainWindow(QWidget* parent): QCustomMainWindow(parent)
{
	SqlLog::saveLog("QDevToys启动");
    QCustomTitleBar* titleBar = new QCustomTitleBar();

    titleBar->setTitleText("QDevToys");

    QObject::connect(titleBar, &QCustomTitleBar::closeButtonClicked, this, &MainWindow::close);
    QObject::connect(titleBar, &QCustomTitleBar::maxButtonClicked, this, &MainWindow::showCustomNormal);
    QObject::connect(titleBar, &QCustomTitleBar::minButtonClicked, this, &MainWindow::showMinimized);
    QObject::connect(this,&MainWindow::windowStateChanged,titleBar,&QCustomTitleBar::updateIcon);
    QObject::connect(titleBar->comboBox, &QComboBox::currentTextChanged, devtoys, &DevToys::onFiterComboBoxTextChanged);
    QObject::connect(titleBar->topButton, &QPushButton::clicked, [=](bool checked)
        {
            // 切换窗口的置顶状态
            if (windowFlags() & Qt::WindowStaysOnTopHint) {
                // 当前是置顶，移除置顶标志
                setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
                titleBar->topButton->setIcon(QIcon(":/DevToys/icon/top.png"));
                titleBar->topButton->setToolTip("置顶");
            }
            else {
                // 当前不是置顶，添加置顶标志
                setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
                titleBar->topButton->setIcon(QIcon(":/DevToys/icon/down.png"));
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
			SqlLog::saveLog(QString("主题切换为%1").arg(theme == 1 ? "深色" : "浅色"));
        }
    );
    QObject::connect(devtoys->settingWidget, &SettingWidget::borderColorChanged, [=](QColor color)
        {
			this->setBorderColor(color, 2);
            this->update();
        });
    QObject::connect(devtoys->settingWidget, &SettingWidget::borderRadiusChanged, [=](int radius) {this->setRadius(radius); });
    this->setTitleBar(titleBar);
    this->setMainWidget(devtoys);
    this->setBorderColor(Qt::GlobalColor::lightGray,2);

    this->setEdgeSize(8);
    this->setWindowOpacity(0.0);
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(500);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
    devtoys->settingWidget->loadUi(this);
}

MainWindow::~MainWindow()
{
	SqlLog::saveLog("QDevToys退出",true);
}

