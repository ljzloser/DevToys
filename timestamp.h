#pragma once

#include <QWidget>
#include "ui_timestamp.h"
#include <qdatetime.h>
#include <qtimezone.h>
#include "tools.h"

/**
 * @brief 时间戳工具窗口类
*/
class Timestamp : public QWidget
{
	Q_OBJECT

public:
	Timestamp(QWidget *parent = nullptr);
	~Timestamp();

private slots:
	/**
	 * @brief 改变最大天数的槽函数
	*/
	void changeMaxDay();
private:
	/**
	 * @brief 设置时间
	 * @param isCurrent 是否为当前时间
	*/
	void setDateTime(bool isCurrent = true);
private:
	Ui::TimestampClass ui;
	void loadConnect();
	void showEvent(QShowEvent* event);
};
