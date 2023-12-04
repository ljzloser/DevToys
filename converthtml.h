#pragma once

#include <QWidget>
#include "ui_converthtml.h"
#include "tools.h"

class ConvertHtml : public QWidget
{
	Q_OBJECT

public:
	ConvertHtml(QWidget *parent = nullptr);
	~ConvertHtml();

private:
	Ui::ConvertHtmlClass ui;
	void loadConnect();
};
