#pragma once
#include "qcustommainwindow.h"
#include "qcustomtitlebar.h"
#include "devtoys.h"
class MainWindow :public QCustomMainWindow//QCustomMainWindow
{
    Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	
};

