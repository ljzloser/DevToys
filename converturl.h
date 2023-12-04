#pragma once

#include <QWidget>
#include "ui_converturl.h"
#include "tools.h"


class ConvertUrl : public QWidget
{
	Q_OBJECT

public:
	ConvertUrl(QWidget *parent = nullptr);
	~ConvertUrl();

private:
	Ui::ConvertUrlClass ui;
	void loadConnect();
};
