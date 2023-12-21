#pragma once

#include <QWidget>
#include "ui_passwordgeneration.h"
#include "tools.h"
#include "randomstringtask.h"
#include <QWaitCondition>
#include <QProgressDialog>

class PasswordGeneration : public QWidget
{
	Q_OBJECT

public:
	/**
	 * PasswordGeneration 类的构造函数。
	 *
	 * @param parent 指向父 QWidget 的指针，默认为 nullptr。
	 */
	PasswordGeneration(QWidget* parent = nullptr);

    /**
	 * PasswordGeneration 类的析构函数。
	 */
	~PasswordGeneration();

private:
	Ui::PasswordGenerationClass ui; // 类的用户界面设置。

	/**
	 * 设置 UI 元素（如按钮和文本字段）的连接。
	 */
	void loadConnect();

	// 插槽声明
private slots:
	/**
	* 当“设置”按钮被点击时触发的插槽。
	* 这应该处理设置某些属性或偏好设置的功能。
	*/
	void onSetButtonClicked();

	/**
	* 当“生成”按钮被点击时触发的插槽。
	* 这应该处理生成密码的功能。
	*/
	void onGenerateButtonClicked();
};
