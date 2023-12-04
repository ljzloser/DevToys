#pragma once

#include <QWidget>
#include "ui_convertbinary.h"
#include "tools.h"


/**
 * @brief 二进制转换窗口类
*/
class ConvertBinary : public QWidget
{
	Q_OBJECT

public:
	ConvertBinary(QWidget *parent = nullptr);
	~ConvertBinary();

private slots:
	void onLineEidtTextChange();

private:
	Ui::ConvertBinaryClass ui;
	void loadConnect();
	/**
	 * @brief 设置文本
	 * @param lineEidt 需要设置的LineEdit 
	 * @param text 需要设置的文本	
	*/
	void setLineEditText(QLineEdit* lineEidt,QString text);
};
