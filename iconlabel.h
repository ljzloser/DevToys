#pragma once

#include <QFrame>
#include "ui_iconlabel.h"
#include <qevent.h>
#include <qpushbutton.h>
/**
 * @brief 图标标签类
*/
class IconLabel : public QFrame
{
	Q_OBJECT

public:
	IconLabel(QWidget *parent = nullptr);
	~IconLabel();
	/**
	 * @brief 设置图标
	 * @param icon  图标
	*/
	void setIcon(QPixmap icon);
	/**
	 * @brief 设置标题
	 * @param title 
	*/
	void setTitle(QString title);
	/**
	 * @brief 设置描述
	 * @param descriptions 
	*/
	void setDescriptions(QString descriptions);

signals:
	/**
	 * @brief 点击信号
	*/
	void clicked(QString name);
private:
	Ui::IconLabelClass ui;
	void enterEvent(QEnterEvent* event);
	void leaveEvent(QEvent* event);
	void mousePressEvent(QMouseEvent* event);
};
