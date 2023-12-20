#pragma once

#include <QWidget>
#include "ui_markdownpreviewwidget.h"

class MarkDownPreviewWidget : public QWidget
{
	Q_OBJECT

public:
	MarkDownPreviewWidget(QWidget* parent = nullptr);
	~MarkDownPreviewWidget();

private:
	Ui::MarkDownPreviewWidgetClass ui;
	void loadConnect();
};
