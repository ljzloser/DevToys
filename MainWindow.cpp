#include "MainWindow.h"

#include <qmainwindow.h>



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

