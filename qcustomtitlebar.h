#ifndef CUSTOMTITLE_H
#define CUSTOMTITLE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QMenu>
#include <QCursor>
#include <QMouseEvent>
#include <qpainter.h>

class QCustomTitleBar : public QWidget
{
	Q_OBJECT

public:
	//按钮图标枚举
	enum ButtonIcon
	{
		MinButtonIcon,//最小化按钮图标
		MaxButtonIcon,//最大化按钮图标
		RestoreButtonIcon,//还原按钮图标
		CloseButtonIcon//关闭按钮图标
	};
	QCustomTitleBar(QWidget* parent = nullptr);
	~QCustomTitleBar();
	//设置标题栏图标
	void setTitleIcon(const QIcon& icon);
	void setTitleIcon(const QPixmap& pixmap);
	//设置标题栏文本
	void setTitleText(const QString& text);
	//设置标题栏菜单
	void setCustomMenu(QMenu* menu);
	//添加菜单项
	QAction* addCustomMenuAction(QAction* action);
	QAction* addCustomMenuAction(const QString& text);
	QAction* addCustomMenuAction(const QIcon& icon, const QString& text);
	//获取标准按钮图标
	QMap<ButtonIcon, QIcon> getStandardIconMap();
	//设置最小化按钮图标
	QIcon setMinButtonIcon(const QIcon& icon);
	QIcon setMinButtonIcon(const QString& iconPath);
	//设置最大化按钮图标
	QIcon setMaxButtonIcon(const QIcon& icon);
	QIcon setMaxButtonIcon(const QString& iconPath);
	//设置还原按钮图标
	QIcon setRestoreButtonIcon(const QIcon& icon);
	QIcon setRestoreButtonIcon(const QString& iconPath);
	//设置关闭按钮图标
	QIcon setCloseButtonIcon(const QIcon& icon);
	QIcon setCloseButtonIcon(const QString& iconPath);
	
	void paintEvent(QPaintEvent* event) override;

signals:
	void minButtonClicked(); //最小化按钮点击信号
	void maxButtonClicked(); //最大化按钮点击信号
	void closeButtonClicked(); //关闭按钮点击信号

protected:
	void mouseDoubleClickEvent(QMouseEvent* event) override;

public slots:
	void updateIcon(); //更新按钮图标
	void showMenu(); //显示菜单

private:
	QHBoxLayout* layout; //标题栏布局
	QIcon icon; //标题栏图标
	QString title; //标题栏文本
	QPushButton* minButton; //最小化按钮
	QPushButton* maxButton; //最大化按钮
	QPushButton* closeButton; //关闭按钮
	QMenu* menu; //标题栏菜单
	bool flag; //最大化按钮标志
	QMap<ButtonIcon, QIcon> standardIconMap; //标准按钮图标
};

#endif // CUSTOMTITLE_H
