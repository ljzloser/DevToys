#pragma once

#include <QWidget>
#include "ui_uuidgeneration.h"
#include "tools.h"
#include <quuid.h>

class UuidGeneration : public QWidget
{
	Q_OBJECT

public:
	UuidGeneration(QWidget* parent = nullptr);
	~UuidGeneration();

private:
	Ui::UuidGenerationClass ui;
	void loadConnect();
private slots:
	/**
	 * @brief 生成按钮点击槽函数
	*/
	void onGenerateButtonClicked();
};
