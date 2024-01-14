#pragma once
#include "qcustommainwindow.h"
#include "qcustomtitlebar.h"
#include "devtoys.h"
class MainWindow :public QCustomMainWindow//QCustomMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
	void loadUi();
	void loadConnect();
	~MainWindow();
	DevToys* devtoys = new DevToys();
	QCustomTitleBar* titleBar = new QCustomTitleBar();
private:
	void keyPressEvent(QKeyEvent* event) override;
};
