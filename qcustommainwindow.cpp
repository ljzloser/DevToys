#include "qcustommainwindow.h"
#include "windows.h"
#include "windowsx.h"
#include <qcoreapplication.h>
#include <qobject.h>
#include <qwindowdefs.h>
#include <qapplication.h>
#include "tools.h"
#pragma comment (lib,"user32.lib")


QCustomMainWindow::QCustomMainWindow(QWidget* parent)
	: QWidget(parent)
{
	this->setWindowFlag(Qt::FramelessWindowHint); //无边框
	this->setAttribute(Qt::WA_TranslucentBackground); //背景透明
	//this->setMouseTracking(true); //鼠标追踪
	//connect(this->timer, &QTimer::timeout, [=]() {
	//	this->updateCursorShape(); //更新光标形状
	//	});
	//this->timer->setParent(this); //设置父对象
	//this->timer->start(1000); //启动定时器
	this->layout->setContentsMargins(this->edge_size, this->edge_size, this->edge_size, this->edge_size); //设置布局边距
	this->layout->setSpacing(0); //设置布局间距
	this->setLayout(this->layout); //设置布局
#ifdef Q_OS_WIN
	HWND hwnd = (HWND)this->winId();
	DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
	::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU);
#endif
	//安装事件过滤器识别拖动
	this->installEventFilter(this);

}

QCustomMainWindow::~QCustomMainWindow()
{
	this->titleBar->deleteLater(); //删除标题栏
	this->menuBar->deleteLater(); //删除菜单栏
	this->mainWidget->deleteLater(); //删除主窗口
	this->statusBar->deleteLater(); //删除状态栏
	delete this->drag_pos; //删除拖动位置
	delete this->customNormal; //删除自定义正常状态
	delete this->background_image; //删除背景图片
	delete this->layout; //删除布局
}

void QCustomMainWindow::loadLayout()
{
	while (this->layout->count())
	{
		this->layout->takeAt(0);//移除布局
	}
	if (this->titleBar != nullptr)
	{
		this->layout->addWidget(this->titleBar); //添加标题栏
	}
	if (this->menuBar != nullptr)
	{
		this->layout->addWidget(this->menuBar); //添加菜单栏
	}
	if (this->mainWidget != nullptr)
	{
		this->layout->addWidget(this->mainWidget); //添加主窗口
	}
	if (this->statusBar != nullptr)
	{
		this->layout->addWidget(this->statusBar); //添加状态栏
	}
}

void QCustomMainWindow::showCustomNormal()
{
	if (this->isMaximized() == false)
	{
		this->showMaximized(); //最大化
	}
	else
	{
		this->showNormal();
	}
	//emit this->windowStateChanged();
}

void QCustomMainWindow::setTitleBar(QWidget* titleBar)
{
	this->titleBar = titleBar;
	this->titleBar->setMouseTracking(true); //设置鼠标跟踪
	this->titleBar->setFixedHeight(25); //设置标题栏高度
	this->titleBar->installEventFilter(this); //安装事件过滤器识别拖动
	this->loadLayout();
}

void QCustomMainWindow::setMenuBar(QWidget* menuBar)
{
	this->menuBar = menuBar;
	this->menuBar->setFixedHeight(30);
	this->loadLayout();
}

void QCustomMainWindow::setMainWidget(QWidget* mainWidget)
{
	this->mainWidget = mainWidget;
	this->mainWidget->installEventFilter(this);
	this->loadLayout();
}

void QCustomMainWindow::setStatusBar(QWidget* statusBar)
{
	this->statusBar = statusBar;
	this->statusBar->setFixedHeight(20);
	this->loadLayout();
}

void QCustomMainWindow::setEdgeSize(int edge_size)
{
	this->edge_size = edge_size;
	this->layout->setContentsMargins(this->edge_size, this->edge_size, this->edge_size, this->edge_size);
}

void QCustomMainWindow::setRadius(int radius)
{
	this->radius = radius;
}

void QCustomMainWindow::setBorderColor(QColor border_color)
{
	this->border_color = border_color;
}

void QCustomMainWindow::setBorderSize(int border_size)
{
	this->border_size = border_size;
}

void QCustomMainWindow::setBackgroundColor(QColor background_color)
{
	this->background_color = background_color;
}

void QCustomMainWindow::paintEvent(QPaintEvent* event)
{

	QPainter* painter = new QPainter(this);
	painter->setRenderHint(QPainter::Antialiasing); //反锯齿
	painter->setPen(Qt::NoPen); //无边框
	painter->setBrush(this->border_color); //边框颜色
	painter->drawRoundedRect(this->rect(), this->radius, this->radius); //绘制边框
	// 再绘制一层背景颜色的圆角矩形，大小为窗口大小减去边框大小
	painter->setBrush(this->background_color); //背景颜色
	painter->drawRoundedRect(this->rect().adjusted(this->border_size, this->border_size,
		-this->border_size, -this->border_size), this->radius, this->radius); //绘制背景
	if (this->background_image != nullptr)
	{
		QImage image = this->background_image->scaled(this->rect().adjusted(
			this->border_size, this->border_size, -this->border_size, -this->border_size).size());
		painter->setBrush(QBrush(image));
		painter->drawRoundedRect(this->rect().adjusted(
			this->border_size, this->border_size, -this->border_size, -this->border_size), this->radius, this->radius);
	}
	for (int i = 1; i < this->layout->count(); i++)
	{
		int x = this->layout->itemAt(i)->geometry().x();
		int y = this->layout->itemAt(i)->geometry().y() - 1;
		int w = this->layout->itemAt(i)->geometry().width();
		int h = 1;
		// 绘制分割线
		painter->setPen(QPen(this->split_line_color, 1));
		painter->drawLine(x, y, x + w, y + h);
	}
	painter->end();
	delete painter;

}

void QCustomMainWindow::updateCursorShape()
{
	/*
	if (!this->isMaximized())
	{
		// 获取窗口的绝对位置
		QRect window_rect = this->geometry();
		// 获取鼠标绝对位置
		QPoint mouse_pos = QCursor::pos();

		if (!this->is_dragging)
		{
			// 判断鼠标位置
			if (mouse_pos.x() < window_rect.x() + this->edge_size)
			{
				// 鼠标在左边缘
				if (mouse_pos.y() < window_rect.y() + this->edge_size)
				{
					// 鼠标在左上角
					this->dragging_edge = "top_left";
					this->setCursor(Qt::CursorShape::SizeFDiagCursor);
				}
				else if (mouse_pos.y() > window_rect.height() - this->edge_size + window_rect.y())
				{
					// 鼠标在左下角
					this->dragging_edge = "bottom_left";
					this->setCursor(Qt::CursorShape::SizeBDiagCursor);
				}
				else
				{
					// 鼠标在左边
					this->dragging_edge = "left";
					this->setCursor(Qt::CursorShape::SizeHorCursor);
				}
			}
			else if (mouse_pos.x() > window_rect.width() - this->edge_size + window_rect.x())
			{
				// 鼠标在右边缘
				if (mouse_pos.y() < window_rect.y() + this->edge_size)
				{
					// 鼠标在右上角
					this->dragging_edge = "top_right";
					this->setCursor(Qt::CursorShape::SizeBDiagCursor);
				}
				else if (mouse_pos.y() > window_rect.height() - this->edge_size + window_rect.y())
				{
					// 鼠标在右下角
					this->dragging_edge = "bottom_right";
					this->setCursor(Qt::CursorShape::SizeFDiagCursor);
				}
				else
				{
					// 鼠标在右边
					this->dragging_edge = "right";
					this->setCursor(Qt::CursorShape::SizeHorCursor);
				}
			}
			else
			{
				// 鼠标在上边缘
				if (mouse_pos.y() < window_rect.y() + this->edge_size)
				{
					// 鼠标在上边
					this->dragging_edge = "top";
					this->setCursor(Qt::CursorShape::SizeVerCursor);
				}
				else if (mouse_pos.y() > window_rect.height() - this->edge_size + window_rect.y())
				{
					// 鼠标在下边
					this->dragging_edge = "bottom";
					this->setCursor(Qt::CursorShape::SizeVerCursor);
				}
				else
				{
					// 鼠标在中间
					this->dragging_edge = nullptr;
					this->setCursor(Qt::CursorShape::ArrowCursor);
				}
			}
		}
	}
	*/
}

void QCustomMainWindow::mouseMoveEvent(QMouseEvent* event)
{
	/*
	if (!this->isMaximized())
	{
		// 获取窗口的绝对位置
		QRect window_rect = this->geometry();
		// 获取鼠标绝对位置
		QPoint mouse_pos = this->mapToGlobal(event->pos());

		if (!this->is_dragging)
		{
			// 判断鼠标位置
			if (mouse_pos.x() < window_rect.x() + this->edge_size)
			{
				// 鼠标在左边缘
				if (mouse_pos.y() < window_rect.y() + this->edge_size)
				{
					// 鼠标在左上角
					this->dragging_edge = "top_left";
					this->setCursor(Qt::CursorShape::SizeFDiagCursor);
				}
				else if (mouse_pos.y() > window_rect.height() - this->edge_size + window_rect.y())
				{
					// 鼠标在左下角
					this->dragging_edge = "bottom_left";
					this->setCursor(Qt::CursorShape::SizeBDiagCursor);
				}
				else
				{
					// 鼠标在左边
					this->dragging_edge = "left";
					this->setCursor(Qt::CursorShape::SizeHorCursor);
				}
			}
			else if (mouse_pos.x() > window_rect.width() - this->edge_size + window_rect.x())
			{
				// 鼠标在右边缘
				if (mouse_pos.y() < window_rect.y() + this->edge_size)
				{
					// 鼠标在右上角
					this->dragging_edge = "top_right";
					this->setCursor(Qt::CursorShape::SizeBDiagCursor);
				}
				else if (mouse_pos.y() > window_rect.height() - this->edge_size + window_rect.y())
				{
					// 鼠标在右下角
					this->dragging_edge = "bottom_right";
					this->setCursor(Qt::CursorShape::SizeFDiagCursor);
				}
				else
				{
					// 鼠标在右边
					this->dragging_edge = "right";
					this->setCursor(Qt::CursorShape::SizeHorCursor);
				}
			}
			else
			{
				// 鼠标在上边缘
				if (mouse_pos.y() < window_rect.y() + this->edge_size)
				{
					// 鼠标在上边
					this->dragging_edge = "top";
					this->setCursor(Qt::CursorShape::SizeVerCursor);
				}
				else if (mouse_pos.y() > window_rect.height() - this->edge_size + window_rect.y())
				{
					// 鼠标在下边
					this->dragging_edge = "bottom";
					this->setCursor(Qt::CursorShape::SizeVerCursor);
				}
				else
				{
					// 鼠标在中间
					this->dragging_edge = nullptr;
					this->setCursor(Qt::CursorShape::ArrowCursor);
				}
			}
		}

		// 判断是否正在拖动
		else
		{
			// 获取窗口最小宽度和高度
			int min_width = this->minimumWidth();
			int min_height = this->minimumHeight();
			// 正在拖动
			if (this->dragging_edge != nullptr)
			{
				// 拖动边缘不为空,根据拖动边缘设置窗口大小,并且考虑最小宽度和高度
				if (this->dragging_edge == "top_left")
				{
					// 左上角
					// 右下角坐标不变
					QRect rect(mouse_pos.x(), mouse_pos.y(), window_rect.width() + window_rect.x() - mouse_pos.x(), window_rect.height() + window_rect.y() - mouse_pos.y());
					// 判断右下角坐标和 window_rect 的右下角坐标是否相同
					if (rect.bottomRight() == window_rect.bottomRight())
					{
						if (rect.width() < min_width)
						{
							rect.setX(rect.x() - min_width + rect.width());
						}
						if (rect.height() < min_height)
						{
							rect.setY(rect.y() - min_height + rect.height());
						}
						this->setGeometry(rect);
					}
				}
				else if (this->dragging_edge == "top")
				{
					// 上边
					// 右下角坐标的 x 不变, y 不变
					QRect rect(window_rect.x(), mouse_pos.y(), window_rect.width(), window_rect.height() + window_rect.y() - mouse_pos.y());
					// 判断右下角坐标和 window_rect 的右下角坐标是否相同
					if (rect.bottomRight() == window_rect.bottomRight())
					{
						if (rect.height() < min_height)
						{
							rect.setY(rect.y() - min_height + rect.height());
						}
						this->setGeometry(rect);
					}
				}
				else if (this->dragging_edge == "top_right")
				{
					// 右上角
					// 左下角坐标的 x 不变, y 不变
					QRect rect(window_rect.x(), mouse_pos.y(), mouse_pos.x() - window_rect.x(), window_rect.height() + window_rect.y() - mouse_pos.y());
					// 判断左下角坐标和 window_rect 的左下角坐标是否相同
					if (rect.bottomLeft() == window_rect.bottomLeft())
					{
						if (rect.width() < min_width)
						{
							rect.setX(window_rect.x());
						}
						if (rect.height() < min_height)
						{
							rect.setY(window_rect.y());
						}
						this->setGeometry(rect);
					}
				}
				else if (this->dragging_edge == "right")
				{
					// 右边
					this->setGeometry(window_rect.x(), window_rect.y(), max(mouse_pos.x() - window_rect.x(), min_width), window_rect.height());
				}
				else if (this->dragging_edge == "bottom_right")
				{
					// 右下角
					this->setGeometry(window_rect.x(), window_rect.y(), max(mouse_pos.x() - window_rect.x(), min_width), max(mouse_pos.y() - window_rect.y(), min_height));
				}
				else if (this->dragging_edge == "bottom")
				{
					// 下边
					this->setGeometry(window_rect.x(), window_rect.y(), window_rect.width(), max(mouse_pos.y() - window_rect.y(), min_height));
				}
				else if (this->dragging_edge == "bottom_left")
				{
					// 左下角
					// 右上角坐标的 x 不变, y 不变
					QRect rect(mouse_pos.x(), window_rect.y(), window_rect.width() + window_rect.x() - mouse_pos.x(), max(mouse_pos.y() - window_rect.y(), min_height));
					// 判断右上角坐标和 window_rect 的右上角坐标是否相同
					if (rect.topRight() == window_rect.topRight())
					{
						if (rect.width() < min_width)
						{
							rect.setX(rect.x() - min_width + rect.width());
						}
						this->setGeometry(rect);
					}
				}
				else if (this->dragging_edge == "left")
				{
					// 左边
					// 右下角坐标的 x 不变, y 不变
					QRect rect(mouse_pos.x(), window_rect.y(), window_rect.width() + window_rect.x() - mouse_pos.x(), window_rect.height());
					// 判断右下角坐标和 window_rect 的右下角坐标是否相同
					if (rect.bottomRight() == window_rect.bottomRight())
					{
						if (rect.width() < min_width)
						{
							rect.setX(rect.x() - min_width + rect.width());
						}
						this->setGeometry(rect);
					}
				}
			}
		}

		if (this->titleBar != nullptr && this->drag_pos != nullptr)
		{
			//标题栏不为空,并且拖动位置不为空
			this->move(this->pos() + (event->pos() - *this->drag_pos));
		}

	}
	*/
}

void QCustomMainWindow::mousePressEvent(QMouseEvent* event)
{
	/*
	if (!this->isMaximized())
	{
		// 不是最大化状态
		if (event->button() == Qt::LeftButton && this->dragging_edge != "")
		{
			// 鼠标左键按下
			this->is_dragging = true;
		}
		if (event->button() == Qt::LeftButton && this->titleBar != nullptr)
		{
			QRect rect = this->titleBar->geometry();
			if (rect.contains(event->pos()))
			{
				this->drag_pos = new QPoint(event->pos());
			}
		}
	}
	*/
}

void QCustomMainWindow::mouseReleaseEvent(QMouseEvent* event)
{
	/*
	if (event->button() == Qt::LeftButton)
	{
		// 鼠标左键释放
		this->is_dragging = false;
		this->drag_pos = nullptr;
		this->dragging_edge = "";
		this->setCursor(Qt::ArrowCursor);
	}
	*/

}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))//Qt6
bool QCustomMainWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
#else
bool QCustomMainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
#endif
{
	if (eventType == "windows_generic_MSG")
	{

		MSG* msg = static_cast<MSG*>(message);//转换类型
		//不同的消息类型和参数进行不同的处理
		if (msg->message == WM_NCCALCSIZE)
		{ //如果是计算窗口大小消息
			*result = 0;
			return true;
		}
		else if (msg->message == WM_SYSKEYDOWN)
		{//如果是alt键按下
			//屏蔽alt键按下
		}
		else if (msg->message == WM_SYSKEYUP)
		{//如果是alt键松开
			//屏蔽alt键松开
		}
		else if (msg->message == WM_NCHITTEST)
		{//如果是鼠标消息

			long x = GET_X_LPARAM(msg->lParam);//获取鼠标x坐标
			long y = GET_Y_LPARAM(msg->lParam);//获取鼠标y坐标
			QPoint pos = mapFromGlobal(QPoint(x, y));
			int padding = this->edge_size;//鼠标距离窗口边缘的距离
			//判断当前鼠标位置在哪个区域
			bool left = pos.x() < padding;
			bool right = pos.x() > width() - padding;
			bool top = pos.y() < padding;
			bool bottom = pos.y() > height() - padding;
			bool resizeEnable = true;//是否允许改变窗口大小
			//鼠标移动到四个角,这个消息是当鼠标移动或者有鼠标键按下时候发出的
			*result = 0;
			if (resizeEnable)
			{
				if (left && top)
				{
					*result = HTTOPLEFT;//
				}
				else if (left && bottom)
				{
					*result = HTBOTTOMLEFT;
				}
				else if (right && top)
				{
					*result = HTTOPRIGHT;
				}
				else if (right && bottom)
				{
					*result = HTBOTTOMRIGHT;
				}
				else if (left)
				{
					*result = HTLEFT;
				}
				else if (right)
				{
					*result = HTRIGHT;
				}
				else if (top)
				{
					*result = HTTOP;
				}
				else if (bottom)
				{
					*result = HTBOTTOM;
				}
			}

			//先处理掉拉伸
			if (0 != *result)
			{
				this->update();
				return true;
			}
			//识别标题栏拖动产生半屏全屏效果
			if (titleBar && titleBar->geometry().contains(pos))
			{ //如果鼠标在标题栏上
				QWidget* child = titleBar->childAt(pos); //获取标题栏上的控件
				if (!child)
				{ //如果标题栏上没有控件
					*result = HTCAPTION; //设置为标题栏
					return true;
				}
				else if (child->objectName() == "maxButton")
				{
					*result = HTMAXBUTTON;
					return true;
				}
			}

		}
		else if (msg->wParam == PBT_APMSUSPEND && msg->message == WM_POWERBROADCAST)
		{
			//系统休眠的时候自动最小化可以规避程序可能出现的问题
			this->showMinimized();
		}
		else if (msg->wParam == PBT_APMRESUMEAUTOMATIC)
		{
			//休眠唤醒后自动打开
			this->showCustomNormal();
		}
		else if (msg->message == WM_NCRBUTTONUP) //如果是鼠标右键
		{

			// 获取系统菜单的句柄
			HMENU hMenu = GetSystemMenu((HWND)this->winId(), FALSE);

			// 获取鼠标位置
			POINT cursor;
			GetCursorPos(&cursor);

			// 设置前台窗口
			SetForegroundWindow((HWND)this->winId());

			// 显示系统菜单
			switch (TrackPopupMenu(hMenu, TPM_LEFTBUTTON | TPM_RETURNCMD, cursor.x, cursor.y, 0, (HWND)this->winId(), NULL))
			{
			case SC_RESTORE: // 还原
				ShowWindow((HWND)this->winId(), SW_RESTORE);
				break;
			case SC_MOVE: // 移动
				SendMessage((HWND)this->winId(), WM_SYSCOMMAND, SC_MOVE, 0);
				break;
			case SC_SIZE: // 大小
				SendMessage((HWND)this->winId(), WM_SYSCOMMAND, SC_SIZE, 0);
				break;
			case SC_MINIMIZE: // 最小化
				ShowWindow((HWND)this->winId(), SW_MINIMIZE);
				break;
			case SC_MAXIMIZE: // 最大化
				ShowWindow((HWND)this->winId(), SW_MAXIMIZE);
				break;
			case SC_CLOSE: // 关闭
				SendMessage((HWND)this->winId(), WM_CLOSE, 0, 0);
				break;
			default:
				break;
			}
			*result = 0;
			// 发送消息给窗口以告知菜单已关闭
			PostMessage((HWND)this->winId(), WM_NULL, 0, 0);
			return true;
		}
		else if (msg->message == WM_SETTINGCHANGE)
		{
			emit this->systemSettingsChanged();
		}
	}
	return false;
}

void QCustomMainWindow::setSplitLineColor(QColor split_line_color)
{
	this->split_line_color = split_line_color;
}

bool QCustomMainWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == this)
	{

		if (event->type() == QEvent::WindowStateChange)
		{
			emit windowStateChanged();
		}


	}
	return QWidget::eventFilter(obj, event);
}

void QCustomMainWindow::resizeEvent(QResizeEvent* event)
{
	this->update();
}
