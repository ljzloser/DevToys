#pragma once
#ifndef QCUSTOMMAINWINDOW_H
#define QCUSTOMMAINWINDOW_H

#include <QtWidgets/QWidget>
#include <qlayout.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qrect.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qbrush.h>
#include <qcolor.h>
#include <qpen.h>
#include <qcursor.h>
#include <qevent.h>

class QCustomMainWindow : public QWidget
{
	Q_OBJECT

public:
	QCustomMainWindow(QWidget* parent = nullptr);
	~QCustomMainWindow();
	//加载布局
	virtual void loadLayout();
	//最大化及还原窗口
	virtual void showCustomNormal();
	//设置标题栏
	virtual void setTitleBar(QWidget* titleBar);
	//设置菜单栏
	virtual void setMenuBar(QWidget* menuBar);
	//设置主窗口
	virtual void setMainWidget(QWidget* mainWidget);
	//设置状态栏
	virtual void setStatusBar(QWidget* statusBar);
	//获取标题栏
	QWidget* getTitleBar() { return this->titleBar; }
	//获取菜单栏
	QWidget* getMenuBar() { return this->menuBar; }
	//获取主窗口
	QWidget* getMainWidget() { return this->mainWidget; }
	//获取状态栏
	QWidget* getStatusBar() { return this->statusBar; }
	//设置边缘大小
	virtual void setEdgeSize(int edge_size);
	//设置圆角半径
	virtual void setRadius(int radius);
	//设置边框颜色
	virtual void setBorderColor(QColor border_color);
	//设置边框大小
	virtual void setBorderSize(int border_size = 2);
	//设置背景颜色
	virtual void setBackgroundColor(QColor background_color);
	//绘图事件
	void paintEvent(QPaintEvent* event);
	//光标更新事件
	virtual void updateCursorShape();
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent* event);
	//鼠标按下事件
	void mousePressEvent(QMouseEvent* event);
	//鼠标释放事件
	void mouseReleaseEvent(QMouseEvent* event);
	//拦截事件
	#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
		bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result);
	#else
		bool nativeEvent(const QByteArray& eventType, void* message, long* result);
	#endif
	//设置分割线颜色
	virtual void setSplitLineColor(QColor split_line_color);
	//事件过滤器
	virtual bool eventFilter(QObject* obj, QEvent* event);
	// 窗口大小改变事件
	virtual void resizeEvent(QResizeEvent* event);
signals:
	void windowStateChanged();
	void systemSettingsChanged();
protected:
	QString dragging_edge = nullptr; //拖动的边缘
	bool is_dragging = false; //是否正在拖动
	int edge_size = 10; //边缘大小
	int radius = 10; //圆角半径
	QColor border_color = Qt::GlobalColor::gray; //边框颜色
	int border_size = 2; //边框大小
	QColor background_color = Qt::GlobalColor::white; //背景颜色
	QImage* background_image = nullptr; //背景图片
	QWidget* titleBar = nullptr; //标题栏
	QWidget* menuBar = nullptr; //菜单栏
	QWidget* mainWidget = nullptr; //主窗口
	QWidget* statusBar = nullptr; //状态栏
	QPoint* drag_pos = nullptr; //拖动位置
	QRect* customNormal = nullptr; //自定义正常状态
	//QTimer* timer = new QTimer(); //定时器
	QVBoxLayout* layout = new QVBoxLayout(); //布局
	//分割线颜色
	QColor split_line_color = Qt::GlobalColor::gray;
};

#endif // QCUSTOMMAINWINDOW_H