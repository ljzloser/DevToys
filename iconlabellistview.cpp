#include "iconlabellistview.h"
#include <qlistview.h>
#include "navigatorview.h"
IconLabelListView::IconLabelListView(QWidget* parent)
	: QListWidget(parent)
{
	this->setViewMode(QListWidget::IconMode);
	this->setFlow(QListView::Flow::LeftToRight);
	// 设置不横向滚动

	this->setResizeMode(QListView::ResizeMode::Adjust);
	this->setSpacing(15);
	// 禁止拖动
	this->setDragEnabled(false);
}

IconLabelListView::~IconLabelListView()
{}

void IconLabelListView::setIconLabels(QStringList names)
{
	this->clear();
	for (QString name : names)
	{
		QListWidgetItem* item = new QListWidgetItem(this);
		IconLabel* label = new IconLabel(this);
		item->setSizeHint(QSize(label->size()));
		label->setTitle(name);
		label->setDescriptions(constants::descriptionsMap.at(name));
		label->setIcon(QPixmap(Tools::formatIconPath(name)));
		connect(label, &IconLabel::clicked, [=]()
			{
				emit this->itemClicked(name);
			});
		this->addItem(item);
		this->setItemWidget(item, label);
	}
}