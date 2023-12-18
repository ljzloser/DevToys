#include "navigatorview.h"

NavigatorView::NavigatorView(QWidget* parent)
	: QFrame(parent)
{
	this->loadUi();
	this->loadConnect();
}

NavigatorView::~NavigatorView()
{}
void NavigatorView::onTreeItemClicked(QModelIndex index)
{
	QStandardItem * item = qobject_cast<QStandardItemModel*>(this->treeView->model())->itemFromIndex(index);
	if (item->hasChildren())
	{
		if (this->treeView->isExpanded(index))
		{
			this->treeView->collapse(index);
		}
		else
		{
			this->treeView->expand(index);
		}	
	}
	if (this->loadTree().count(item->text()))
	{
		emit this->parentItemClicked(this->loadTree()[item->text()]);
	}
	else
	{
		emit this->itemClicked(item->text());
	}
}
void NavigatorView::setItemHeight(int height)
{
	this->m_ItemHeight = height;
	this->delegate->setItemHeight(height);
}

int NavigatorView::ItemHeight() const
{
	return this->m_ItemHeight;
}

void NavigatorView::setItemWidth(int width)
{
	this->m_ItemWidth = width;
	this->treeView->setColumnWidth(0, width);
	QFont font("Microsoft YaHei");
	font.setPixelSize(this->ItemHeight() / 10 * 6);
	this->treeView->setFont(font);
}
int NavigatorView::ItemWidth() const
{
	return this->m_ItemWidth;
}

void NavigatorView::loadUi()
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	// 只读
	this->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	// 设置列宽
	this->treeView->setColumnWidth(0, 30);
	this->treeView->setUniformRowHeights(true);
	QFont font("Microsoft YaHei");
	font.setPixelSize(this->ItemHeight() / 10 * 6);
	this->treeView->setFont(font);
	this->treeView->setAnimated(true);
	this->treeView->setItemDelegate(this->delegate);
	QStandardItemModel* model = new QStandardItemModel();
	this->treeView->setModel(model);
	model->setHorizontalHeaderLabels({ "Icon" });
	this->treeView->setHeaderHidden(true);
	std::map<QString, QStringList> treeMap = this->loadTree();
	QStandardItem* root = model->invisibleRootItem();
	for (auto& item : treeMap)
	{
		QString key = item.first;
		QStringList values = item.second;
		QStandardItem* parent = new QStandardItem(QIcon(Tools::formatIconPath(key)), key);
		root->appendRow(parent);
		for (auto& subItem : values)
		{
			QString value = Tools::formatIconPath(subItem);
			QStandardItem* child = new QStandardItem(QIcon(Tools::formatIconPath(subItem)), subItem);
			parent->appendRow(child);
		}
	}
	layout->addWidget(this->allToolsButton);
	layout->addWidget(this->treeView);
	layout->addWidget(this->setButton);
	this->setButton->setIcon(QIcon(":/DevToys/icon/设置.png"));
	this->setButton->setText("设置");
	// 左对齐
	this->setButton->setStyleSheet("QPushButton { text-align: left; padding-left: 5px; }");

	this->allToolsButton->setText("全部工具");
	this->allToolsButton->setIcon(QIcon(":/DevToys/icon/home.png"));
	this->allToolsButton->setStyleSheet("QPushButton { text-align: left; padding-left: 5px; }");

	this->setItemHeight(40);
	this->setLayout(layout);
}

void NavigatorView::loadConnect()
{
	connect(this->treeView, &QTreeView::clicked, this, &NavigatorView::onTreeItemClicked);
}

std::map<QString, QStringList> NavigatorView::loadTree()
{
	return treeMap;
}


