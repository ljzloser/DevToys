#pragma once

#include <QWidget>
#include "ui_jsonstringformat.h"
#include <json/json.h>
#include "tools.h"

class JsonStringFormat : public QWidget
{
	Q_OBJECT

public:
	JsonStringFormat(QWidget *parent = nullptr);
	~JsonStringFormat();

private:
	Ui::JsonStringFormatClass ui;
	void loadConnect();
	
private slots:
	void onImportWidgetTextChanged();
};
