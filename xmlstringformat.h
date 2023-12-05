#pragma once

#include <QWidget>
#include "ui_xmlstringformat.h"
#include "tools.h"
#include <qdom.h>
#include <Qsci/qscilexerxml.h>

class XmlStringFormat : public QWidget
{
	Q_OBJECT

public:
	XmlStringFormat(QWidget *parent = nullptr);
	~XmlStringFormat();

private:
	Ui::XmlStringFormatClass ui;
	void loadConnect();

private slots:
	void onImportWidgetTextChanged();
};
