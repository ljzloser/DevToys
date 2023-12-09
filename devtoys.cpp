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
	this->setWindowOpacity(0.0);
	QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
	animation->setDuration(500);
	animation->setStartValue(0.0);
	animation->setEndValue(1.0);
	animation->start(QPropertyAnimation::DeleteWhenStopped);

	this->jsonToYaml->setObjectName("JSON-YAML数据类型互转工具");
	this->timestamp->setObjectName("Unix时间戳转换工具");
	this->convertBinary->setObjectName("进制转换工具");
	this->convertHtml->setObjectName("HTML编码解码工具");
	this->convertUrl->setObjectName("URL编码解码工具");
	this->convertBase64->setObjectName("Base64文本编码解码工具");
	this->convertBase64Image->setObjectName("Base64图像编码解码工具");
	this->jsonStringFormat->setObjectName("JSON字符串格式化工具");
	this->xmlStringFormat->setObjectName("可扩展标记语言XML格式化工具");
	this->stringHashGeneration->setObjectName("哈希散列生成工具");
	this->passwordGeneration->setObjectName("密码生成器");
	this->uuidGeneration->setObjectName("通用唯一识别码生成工具");

	QWidget* widget = new QWidget(this);
	widget->setLayout(this->stackedLayout);

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
	this->stackedLayout->addWidget(this->xmlStringFormat);
	this->stackedLayout->addWidget(this->stringHashGeneration);
	this->stackedLayout->addWidget(this->passwordGeneration);
	this->stackedLayout->addWidget(this->uuidGeneration);


	splitter->addWidget(this->navigator);
	splitter->addWidget(widget);
	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget(this->splitter);
	this->setLayout(layout);

	for (int i = 0; i < this->stackedLayout->count(); i++)
	{
		QWidget* layoutWidget = this->stackedLayout->widget(i);
		AnimationOpacityEffect* opacityEffect = new AnimationOpacityEffect(this);
		layoutWidget->setGraphicsEffect(opacityEffect);
	}

}

void DevToys::showToolWidget(QString name)
{
	// 根据objectName找到对应的widget
	QWidget* widget = this->findChild<QWidget*>(name);
	if (widget)
	{
		// 从widget的graphicsEffect中取出GraphicsOpacityEffect并通过static_cast转换为AnimationOpacityEffect
		AnimationOpacityEffect* opacityEffect = static_cast<AnimationOpacityEffect*>(widget->graphicsEffect());
		opacityEffect->setOpacity(0.0);
		this->stackedLayout->setCurrentWidget(widget);
		opacityEffect->inAnimationStart();
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
			AnimationOpacityEffect* opacityEffect = static_cast<AnimationOpacityEffect*>(this->listView->graphicsEffect());
			opacityEffect->setOpacity(0.0);
			this->listView->setIconLabels(names);
			this->stackedLayout->setCurrentIndex(0);
			opacityEffect->inAnimationStart();
		});
	connect(this->navigator, &NavigatorView::itemClicked, this, &DevToys::showToolWidget);
	connect(this->listView, &IconLabelListView::itemClicked, this, &DevToys::showToolWidget);
}
