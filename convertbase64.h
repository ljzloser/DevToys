#pragma once

#include <QWidget>
#include "ui_convertbase64.h"
#include "tools.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

class ConvertBase64 : public QWidget
{
	Q_OBJECT

public:
	ConvertBase64(QWidget* parent = nullptr);
	~ConvertBase64();

private:
	Ui::ConvertBase64Class ui;
	void loadConnect();
};
