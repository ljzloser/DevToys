#include "animationopacityeffect.h"
#include "constants.h"
#include "devtoys.h"
#include "iconlabellistview.h"
#include "navigatorview.h"
#include <qabstractitemmodel.h>
#include <qapplication.h>
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qdir.h>
#include <QImageWriter>
#include <qitemselectionmodel.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <qstandarditemmodel.h>
#include <qstring.h>
#include <qtreeview.h>
#include <qwidget.h>
#include <utility>

DevToys::DevToys(QWidget* parent)
	: QWidget(parent)
{
	this->loadUi();
	this->loadConnect();
	this->navigator->getAllToolsButton()->click();
	// 获取并打印所有支持的图像读取格式
	QList<QByteArray> readFormats = QImageReader::supportedImageFormats();
	qDebug() << "Supported read formats:" << readFormats.join(", ");

	// 获取并打印所有支持的图像写入格式
	QList<QByteArray> writeFormats = QImageWriter::supportedImageFormats();
	qDebug() << "Supported write formats:" << writeFormats.join(", ");
}

DevToys::~DevToys()
{
	// 清理临时文件
	QString Dir = QApplication::applicationDirPath() + "\\temp";
	if (QDir(Dir).exists())
	{
		// 清理临时文件
		QDir(Dir).removeRecursively();
	}
}

void DevToys::onFiterComboBoxTextChanged(QString text)
{
	if (text.isEmpty())
	{
		return;
	}
	QTreeView* treeView = this->navigator->getTree();
	QStandardItemModel* model = static_cast<QStandardItemModel*>(treeView->model());
	bool isFind = false;
	// 获取选择模型
	QItemSelectionModel* selectionModel = treeView->selectionModel();
	// 清除所有选中的项
	selectionModel->clearSelection();
	// 遍历所有顶级项
	for (int row = 0; row < model->rowCount(); ++row)
	{
		QStandardItem* parentItem = model->item(row);

		// 遍历第二级项
		for (int childRow = 0; childRow < parentItem->rowCount(); ++childRow)
		{
			QStandardItem* childItem = parentItem->child(childRow);

			// 检查是否匹配
			if (childItem->text() == text)
			{
				// 构建 QModelIndex
				QModelIndex indexToSelect = model->index(childRow, 0, parentItem->index());

				// 选中匹配的项
				selectionModel->select(indexToSelect, QItemSelectionModel::Select | QItemSelectionModel::Rows);

				// 滚动到选中的项
				treeView->scrollTo(indexToSelect);
				treeView->clicked(indexToSelect);
				return;
			}
		}
	}
	// 如果没有找到匹配项。说明没在树中
	this->showToolWidget(text);
}

void DevToys::loadUi()
{
	QWidget* widget = new QWidget(this);
	widget->setLayout(this->stackedLayout);
	this->stackedLayout->setContentsMargins(0, 0, 0, 0);
	addWidgetToLayout("列表", this->listView);
	addWidgetToLayout("其他", this->gaugeBoardWidget);
	addWidgetToLayout("JSON-YAML数据类型互转工具", this->jsonToYaml);
	addWidgetToLayout("Unix时间戳转换工具", this->timestamp);
	addWidgetToLayout("进制转换工具", this->convertBinary);
	addWidgetToLayout("HTML编码解码工具", this->convertHtml);
	addWidgetToLayout("URL编码解码工具", this->convertUrl);
	addWidgetToLayout("Base64文本编码解码工具", this->convertBase64);
	addWidgetToLayout("Base64图像编码解码工具", this->convertBase64Image);
	addWidgetToLayout("JSON字符串格式化工具", this->jsonStringFormat);
	addWidgetToLayout("可扩展标记语言XML格式化工具", this->xmlStringFormat);
	addWidgetToLayout("哈希散列生成工具", this->stringHashGeneration);
	addWidgetToLayout("密码生成器", this->passwordGeneration);
	addWidgetToLayout("通用唯一识别码生成工具", this->uuidGeneration);
	addWidgetToLayout("文本转义-反转义", this->escapeStringWidget);
	addWidgetToLayout("MarkDown预览工具", this->markDownPreviewWidget);
	addWidgetToLayout("简易SqlLite查询工具", this->sqlLiteIDE);
	addWidgetToLayout("系统信息", this->systemInfoWidget);
	addWidgetToLayout("设置", this->settingWidget);

	splitter->addWidget(this->navigator);
	splitter->addWidget(widget);
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(5, 5, 5, 5);
	layout->addWidget(this->splitter);
	this->setLayout(layout);
}

void DevToys::showToolWidget(QString name)
{
	// 根据objectName找到对应的widget
	QWidget* widget = this->findChild<QWidget*>(name);
	if (widget)
	{
		if (this->stackedLayout->currentWidget() == widget)// 判断当前widget是否是正在显示的,防止反复切换
			return;
		// 从widget的graphicsEffect中取出GraphicsOpacityEffect并通过static_cast转换为AnimationOpacityEffect
		AnimationOpacityEffect* opacityEffect = static_cast<AnimationOpacityEffect*>(widget->graphicsEffect());
		opacityEffect->setOpacity(0.0);
		this->stackedLayout->setCurrentWidget(widget);
		if (!constants::descriptionsMap.count(name))
			this->navigator->getTree()->selectionModel()->clearSelection();
		// 判断sender 是不是NavigatorView
		if (!qobject_cast<NavigatorView*>(sender()))
			this->onFiterComboBoxTextChanged(widget->objectName());
		opacityEffect->inAnimationStart();
	}
	else
	{
		AnimationOpacityEffect* opacityEffect = static_cast<AnimationOpacityEffect*>(
			this->gaugeBoardWidget->graphicsEffect());
		opacityEffect->setOpacity(0.0);
		this->stackedLayout->setCurrentIndex(1);
		opacityEffect->inAnimationStart();
	}
}

void DevToys::loadConnect()
{
	connect(navigator, &NavigatorView::parentItemClicked, this, &DevToys::onParentItemClicked);
	connect(this->navigator, &NavigatorView::itemClicked, this, &DevToys::showToolWidget);
	connect(this->listView, &IconLabelListView::itemClicked, this, &DevToys::showToolWidget);
	connect(this->navigator->getSetButton(), &QPushButton::clicked, [=]() {showToolWidget(this->settingWidget->objectName()); });
	connect(this->navigator->getAllToolsButton(), &QPushButton::clicked, [=]() {
		this->navigator->getTree()->selectionModel()->clearSelection();
		QStringList names;
		for (std::pair<const QString, QString> var : constants::descriptionsMap)
		{
			names.append(var.first);
		}
		this->onParentItemClicked(names);
		});
}

void DevToys::addWidgetToLayout(QString name, QWidget* widget)
{
	widget->setObjectName(name);
	AnimationOpacityEffect* opacityEffect = new AnimationOpacityEffect(widget);
	widget->setGraphicsEffect(opacityEffect);
	this->stackedLayout->addWidget(widget);
}

void DevToys::onParentItemClicked(QStringList& names)
{
	AnimationOpacityEffect* opacityEffect = static_cast<AnimationOpacityEffect*>(this->listView->graphicsEffect());
	opacityEffect->setOpacity(0.0);
	this->listView->setIconLabels(names);
	this->stackedLayout->setCurrentIndex(0);
	opacityEffect->inAnimationStart();
}