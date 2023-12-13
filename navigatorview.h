#pragma once

#include <QWidget>
#include <qlistview.h>
#include <qtreeview.h>
#include <qlayout.h>
#include <qitemdelegate.h>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include "constants.h"
#include "tools.h"


/**
 * @brief 用于NavigatorView的列高委托
*/
class Delegate : public QItemDelegate
{
public:
	Delegate(QObject* parent = 0, int height = 30) : QItemDelegate(parent) { this->height = height; }
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		QSize size = QItemDelegate::sizeHint(option, index);
		size.setHeight(this->height);
		return size;
	}
	/**
	 * @brief 设置列高
	 * @param height 
	*/
	void setItemHeight(int height) { this->height = height; }
private:
	int height = 30;
};
/**
 * @brief 导航视图窗口
*/
class NavigatorView : public QWidget
{
	Q_OBJECT

public:

	NavigatorView(QWidget* parent = nullptr);
	~NavigatorView();
	/**
	 * @brief 设置列高
	 * @param height 
	*/
	void setItemHeight(int height);
	/**
	 * @brief 获取列高
	 * @return  
	*/
	int ItemHeight() const;
	/**
	 * @brief 设置列宽
	 * @param width 
	*/
	void setItemWidth(int width);
	/**
	 * @brief 获取列宽
	 * @return 
	*/
	int ItemWidth() const;

	QTreeView* getTree() const { return treeView; }

private slots:
	/**
	 * @brief 单击树Item槽函数
	*/
	void onTreeItemClicked(QModelIndex index);

signals:
	/**
	 * @brief 单击父树Item信号
	*/
	void parentItemClicked(QStringList names);
signals:
	/**
	 * @brief 单击子树Item信号
	*/
	void itemClicked(QString name);
private:
	/**
	 * @brief 加载UI
	*/
	void loadUi();
	/**
	 * @brief 加载信号
	*/
	void loadConnect();

	// 树控件
	QTreeView* treeView = new QTreeView(this);
	// 列高
	int m_ItemHeight = 30;
	// 列宽
	int m_ItemWidth = 100;
	//设置委托
	Delegate* delegate = new Delegate(this->treeView);
	// 数Map
	std::map<QString, QStringList> loadTree();


};
