#include "qcustomtitlebar.h"
#include <QLineEdit>
#include <qpainterpath.h>
#include "filtercombobox.h"
#include "constants.h"

QCustomTitleBar::QCustomTitleBar(QWidget* parent) : QWidget(parent)
{
	this->layout = new QHBoxLayout(this);
	this->layout->setContentsMargins(0, 0, 0, 0);

	/*自动缩放的占位符*/
	QSpacerItem* spacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	QStringList list;
	for (std::pair<const QString, QString> var : descriptionsMap)
	{
		list.append(var.first);
	}
	comboBox->setItems(list);

	this->topButton = new QPushButton(this);
	this->topButton->setFixedSize(25, 25);
	this->topButton->setCheckable(true);
	this->topButton->setChecked(false);
	this->topButton->setIcon(QIcon(":/DevToys/icon/top.png"));
	this->topButton->setToolTip("置顶");

	//再添加一个占位符，使得搜索框居中
	QSpacerItem* spacerItem2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	//初始化按钮图标
	this->standardIconMap.insert(ButtonIcon::MinButtonIcon, style()->standardIcon(QStyle::SP_TitleBarMinButton));
	this->standardIconMap.insert(ButtonIcon::MaxButtonIcon, style()->standardIcon(QStyle::SP_TitleBarMaxButton));
	this->standardIconMap.insert(ButtonIcon::RestoreButtonIcon, style()->standardIcon(QStyle::SP_TitleBarNormalButton));
	this->standardIconMap.insert(ButtonIcon::CloseButtonIcon, style()->standardIcon(QStyle::SP_TitleBarCloseButton));

	this->setContextMenuPolicy(Qt::CustomContextMenu);//设置标题栏为自定义上下文菜单策略
	connect(this, &QCustomTitleBar::customContextMenuRequested, this, &QCustomTitleBar::showMenu);//右键菜单信号.显示菜单
	this->menu = new QMenu(this);

	this->minButton = new QPushButton(this);
	this->minButton->setFixedSize(25, 25);
	this->minButton->setIcon(this->standardIconMap.value(ButtonIcon::MinButtonIcon));
	this->minButton->setCursor(Qt::PointingHandCursor);//设置鼠标为手型
	connect(this->minButton, &QPushButton::clicked, this, &QCustomTitleBar::minButtonClicked);//最小化按钮信号
	this->minButton->setToolTip("最小化");

	this->maxButton = new QPushButton(this);
	this->maxButton->setFixedSize(25, 25);
	this->maxButton->setIcon(this->standardIconMap.value(ButtonIcon::MaxButtonIcon));
	this->maxButton->setCursor(Qt::PointingHandCursor);
	//connect(this->maxButton, &QPushButton::clicked, this, &QCustomTitleBar::updateIcon); //最大化按钮信号,切换图标
	connect(this->maxButton, &QPushButton::clicked, this, &QCustomTitleBar::maxButtonClicked);
	//设置悬停提示
	this->maxButton->setToolTip("最大化/还原");
	this->maxButton->setObjectName("maxButton");

	this->closeButton = new QPushButton(this);
	this->closeButton->setFixedSize(25, 25);
	this->closeButton->setIcon(this->standardIconMap.value(ButtonIcon::CloseButtonIcon));
	this->closeButton->setCursor(Qt::PointingHandCursor);
	connect(this->closeButton, &QPushButton::clicked, this, &QCustomTitleBar::closeButtonClicked);
	this->closeButton->setToolTip("关闭");

	this->layout->addSpacerItem(spacerItem);
	this->layout->addWidget(comboBox);
	this->layout->addWidget(this->topButton);
	this->layout->addSpacerItem(spacerItem2);
	this->layout->addWidget(this->minButton);
	this->layout->addWidget(this->maxButton);
	this->layout->addWidget(this->closeButton);

	this->setLayout(this->layout);
	this->setTitleText("Title");
	this->icon = style()->standardIcon(QStyle::SP_TitleBarMenuButton);
	this->flag = false;

	QAction* minimizeAction = this->menu->addAction("最小化");
	minimizeAction->setObjectName("minimizeAction");
	minimizeAction->setIcon(this->standardIconMap.value(ButtonIcon::MaxButtonIcon));
	connect(minimizeAction, &QAction::triggered, this, &QCustomTitleBar::minButtonClicked);

	QAction* maximizeAction = this->menu->addAction("最大化/还原");
	maximizeAction->setObjectName("maximizeAction");
	maximizeAction->setIcon(this->standardIconMap.value(ButtonIcon::MaxButtonIcon));
	//connect(maximizeAction, &QAction::triggered, this, &QCustomTitleBar::maxButtonClicked);
	connect(maximizeAction, &QAction::triggered, this, &QCustomTitleBar::updateIcon);

	QAction* quitAction = this->menu->addAction("退出");
	quitAction->setObjectName("quitAction");
	quitAction->setIcon(this->standardIconMap.value(ButtonIcon::CloseButtonIcon));
	connect(quitAction, &QAction::triggered, this, &QCustomTitleBar::closeButtonClicked);
}

QCustomTitleBar::~QCustomTitleBar()
{
	delete this->layout;

	delete this->minButton;
	delete this->maxButton;
	delete this->closeButton;
	delete this->menu;
}

void QCustomTitleBar::setTitleIcon(const QIcon& icon)
{
	this->icon = icon;
}

void QCustomTitleBar::setTitleIcon(const QPixmap& pixmap)
{
	this->icon = QIcon(pixmap);
}

void QCustomTitleBar::setTitleText(const QString& text)
{
	this->title = text;
}

void QCustomTitleBar::updateIcon()
{
	if (this->flag) {
		this->maxButton->setIcon(this->standardIconMap.value(ButtonIcon::MaxButtonIcon));
		for (QAction* action : menu->actions()) {
			if (action->objectName() == "maximizeAction") {
				action->setIcon(this->standardIconMap.value(ButtonIcon::MaxButtonIcon));

				continue;
			}
		}
		/*		QCustomTitleBar::maxButtonClicked()*/;
		this->flag = false;
	}
	else {
		this->maxButton->setIcon(this->standardIconMap.value(ButtonIcon::RestoreButtonIcon));
		for (QAction* action : menu->actions()) {
			if (action->objectName() == "maximizeAction") {
				action->setIcon(this->standardIconMap.value(ButtonIcon::RestoreButtonIcon));
				continue;
			}
		}
		//还原按钮点击

		this->flag = true;
	}
	update();
}

void QCustomTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		this->maxButton->click();
		return QWidget::mouseDoubleClickEvent(event);
	}
}

void QCustomTitleBar::setCustomMenu(QMenu* menu)
{
	this->menu = menu;
}

QAction* QCustomTitleBar::addCustomMenuAction(QAction* action)
{
	this->menu->addAction(action);
	return action;
}

QAction* QCustomTitleBar::addCustomMenuAction(const QString& text)
{
	QAction* action = menu->addAction(text);
	return action;
}

QAction* QCustomTitleBar::addCustomMenuAction(const QIcon& icon, const QString& text)
{
	QAction* action = menu->addAction(icon, text);
	return action;
}

QMap<QCustomTitleBar::ButtonIcon, QIcon> QCustomTitleBar::getStandardIconMap()
{
	return this->standardIconMap;
}

QIcon QCustomTitleBar::setMinButtonIcon(const QIcon& icon)
{
	// 更新最小化按钮图标
	this->standardIconMap.insert(ButtonIcon::MaxButtonIcon, icon);
	this->minButton->setIcon(icon);
	for (QAction* action : this->menu->actions()) {
		if (action->objectName() == "minimizeAction") {
			action->setIcon(icon);
			break;
		}
	}
	return icon;
}

QIcon QCustomTitleBar::setMinButtonIcon(const QString& iconPath)
{
	QIcon icon(iconPath);
	return this->setMinButtonIcon(icon);;
}

QIcon QCustomTitleBar::setMaxButtonIcon(const QIcon& icon)
{
	// 更新最大化按钮图标
	this->standardIconMap.insert(ButtonIcon::MaxButtonIcon, icon);
	this->maxButton->setIcon(icon);
	for (QAction* action : this->menu->actions()) {
		if (action->objectName() == "maximizeAction") {
			action->setIcon(icon);
			break;
		}
	}
	return icon;
}

QIcon QCustomTitleBar::setMaxButtonIcon(const QString& iconPath)
{
	QIcon icon(iconPath);
	return this->setMaxButtonIcon(icon);
}

QIcon QCustomTitleBar::setRestoreButtonIcon(const QIcon& icon)
{
	// 更新还原按钮图标
	this->standardIconMap.insert(ButtonIcon::RestoreButtonIcon, icon);
	this->maxButton->setIcon(icon);
	for (QAction* action : this->menu->actions()) {
		if (action->objectName() == "maximizeAction") {
			action->setIcon(icon);
			break;
		}
	}
	return icon;
}

QIcon QCustomTitleBar::setRestoreButtonIcon(const QString& iconPath)
{
	QIcon icon(iconPath);
	return this->setRestoreButtonIcon(icon);
}

QIcon QCustomTitleBar::setCloseButtonIcon(const QIcon& icon)
{
	// 更新关闭按钮图标
	this->standardIconMap.insert(ButtonIcon::CloseButtonIcon, icon);
	this->closeButton->setIcon(icon);
	for (QAction* action : this->menu->actions()) {
		if (action->objectName() == "quitAction") {
			action->setIcon(icon);
			break;
		}
	}
	return icon;
}

QIcon QCustomTitleBar::setCloseButtonIcon(const QString& iconPath)
{
	QIcon icon(iconPath);
	return this->setCloseButtonIcon(icon);
}

void QCustomTitleBar::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//绘制icon,大小为30，30
	painter.drawPixmap(0, 0, 25, 25, this->icon.pixmap(25, 25));
	// 字体大小
	QFont font;
	font.setPointSize(12);
	//微软雅黑
	font.setFamily("Microsoft YaHei");
	painter.setFont(font);

	// 绘制标题
	painter.drawText(40, 0, this->width() - 40, 25, Qt::AlignLeft | Qt::AlignVCenter, this->title);
	painter.end();
	update();
}

void QCustomTitleBar::showMenu()
{
	//this->menu->exec(QCursor::pos());
}