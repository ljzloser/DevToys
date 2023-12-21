#pragma once

#include "tools.h"
#include "ui_stringhashgeneration.h"
#include <QCryptographicHash>
#include <QWidget>

class StringHashGeneration : public QWidget
{
	Q_OBJECT

public:
	StringHashGeneration(QWidget* parent = nullptr);
	~StringHashGeneration();

private:
	Ui::StringHashGenerationClass ui;
	void loadConnect();
private slots:
	void onImportTextEditChanged();
};
