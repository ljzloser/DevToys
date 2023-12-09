#pragma once

#include <QWidget>
#include "ui_stringhashgeneration.h"
#include "tools.h"
#include <QCryptographicHash>


class StringHashGeneration : public QWidget
{
	Q_OBJECT

public:
	StringHashGeneration(QWidget *parent = nullptr);
	~StringHashGeneration();

private:
	Ui::StringHashGenerationClass ui;
	void loadConnect();
private slots:
	void onImportTextEditChanged();
};
