#include "MainWindow.h"

#include <qmainwindow.h>

#include "windows.h"
#include "windowsx.h"
#pragma comment (lib,"user32.lib")

MainWindow::MainWindow(QWidget* parent): QCustomMainWindow(parent)
{
    QCustomTitleBar* titleBar = new QCustomTitleBar();

    titleBar->setTitleText("QDevToys");
    titleBar->setTitleIcon(QIcon(":/DevToys/icon/生成类型工具.png"));
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

    this->setTitleBar(titleBar);


    this->setMainWidget(devtoys);

    this->setBorderColor(Qt::GlobalColor::lightGray,2);
    //背景色为#19232D
    this->setBackgroundColor(QColor("#19232D"));
    //分割线颜色
    this->setSplitLineColor(QColor("#19232D"));
    this->setEdgeSize(8);
    this->setWindowOpacity(0.0);
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(500);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

MainWindow::~MainWindow()
{
}

