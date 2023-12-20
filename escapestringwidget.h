#pragma once

#include <QWidget>
#include "ui_escapestringwidget.h"
#include "tools.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

class EscapeStringWidget : public QWidget
{
	Q_OBJECT

public:
	EscapeStringWidget(QWidget* parent = nullptr);
	~EscapeStringWidget();

private:
	Ui::EscapeStringWidgetClass ui;
	void loadConnect();
};
