#pragma once

#include <QListWidget>
#include "iconlabel.h"
#include "tools.h"
#include "constants.h"

/**
 * @brief 图标标签列表
*/
class IconLabelListView : public QListWidget
{
	Q_OBJECT

public:
	IconLabelListView(QWidget *parent = nullptr);
	~IconLabelListView();
	/**
	 * @brief 设置图标标签类,IconLabel
	 * @param names 
	*/
	void setIconLabels(QStringList names);
signals:
	/**
	 * @brief 点击了图标标签
	*/
	void itemClicked(QString name);
private:
};
