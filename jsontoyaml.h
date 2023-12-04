#pragma once

#include "tools.h"
#include "ui_JsonToYaml.h"
#include <json/json.h>
#include <QFileDialog>
#include <QWidget>
#include <yaml-cpp/yaml.h>

/**
 * @brief Json/Yaml转换窗口类
*/
class JsonToYaml : public QWidget
{
	Q_OBJECT

public:
	JsonToYaml(QWidget* parent = nullptr);
	~JsonToYaml();



public slots:
	/**
	 * @brief 转换ComboBox的索引改变
	 * @param index
	*/
	void onConvertComboBoxCurrentIndexChanged(int index);
	/**
	 * @brief 缩进ComboBox的索引改变
	 * @param index
	*/
	void onIndentComboBoxCurrentIndexChanged(int index);
	/**
	 * @brief 转换前的文本改变
	*/
	void onImportChanged();

private:
	Ui::JsonToYaml ui;
	void setUi();
	void loadConnect();
};
