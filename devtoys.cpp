#include "devtoys.h"
#include "qtimezone.h"
DevToys::DevToys(QWidget* parent)
	: QWidget(parent)
{
	this->loadUi();
	this->loadConnect();
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

void DevToys::loadUi()
{
	this->jsonToYaml->setObjectName("JSON-YAML数据类型互转工具");
	this->timestamp->setObjectName("Unix时间戳转换工具");
	this->convertBinary->setObjectName("进制转换工具");
	this->convertHtml->setObjectName("HTML编码解码工具");
	this->convertUrl->setObjectName("URL编码解码工具");
	this->convertBase64->setObjectName("Base64文本编码解码工具");
	this->convertBase64Image->setObjectName("Base64图像编码解码工具");
	this->jsonStringFormat->setObjectName("JSON字符串格式化工具");

	this->stackedLayout->setContentsMargins(0, 0, 0, 0);
	this->stackedLayout->addWidget(this->listView);
	this->stackedLayout->addWidget(this->label);
	this->stackedLayout->addWidget(this->jsonToYaml);
	this->stackedLayout->addWidget(this->timestamp);
	this->stackedLayout->addWidget(this->convertBinary);
	this->stackedLayout->addWidget(this->convertHtml);
	this->stackedLayout->addWidget(this->convertUrl);
	this->stackedLayout->addWidget(this->convertBase64);
	this->stackedLayout->addWidget(this->convertBase64Image);
	this->stackedLayout->addWidget(this->jsonStringFormat);

	QWidget* widget = new QWidget();
	widget->setLayout(this->stackedLayout);
	splitter->addWidget(this->navigator);
	splitter->addWidget(widget);
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(this->splitter);
	this->setLayout(layout);
}

void DevToys::showToolWidget(QString name)
{
	// 根据objectName找到对应的widget
	QWidget* widget = this->findChild<QWidget*>(name);
	if (widget)
	{
		this->stackedLayout->setCurrentWidget(widget);
	}
	else
	{
		this->stackedLayout->setCurrentIndex(1);
		this->label->setText(name + "正在开发啦！！！");
	}
}

void DevToys::loadConnect()
{
	connect(navigator, &NavigatorView::parentItemClicked, [=](QStringList names)
		{
			this->listView->setIconLabels(names);
			this->stackedLayout->setCurrentIndex(0);
		});
	connect(this->navigator, &NavigatorView::itemClicked, this, &DevToys::showToolWidget);
	connect(this->listView, &IconLabelListView::itemClicked, this, &DevToys::showToolWidget);
}
