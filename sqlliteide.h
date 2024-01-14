#pragma once

#include "Qsci/qscilexersql.h"
#include "SqlExecutor.h"
#include "ui_sqlliteide.h"
#include <qmenubar.h>
#include <qtreewidget.h>
#include <qvariant.h>
#include <QWidget>

#include <qcombobox.h>

#include "qplaintextedit.h"

#include "qtableview.h"

#include "qevent.h"

class SqlLiteIDE : public QWidget
{
	Q_OBJECT
private:
	enum ExecuteType
	{
		Query, //select
		NonQuery, //insert, update, delete
	};
public:
	void loadBaseApis();
	SqlLiteIDE(QWidget* parent = nullptr);
	~SqlLiteIDE();

private:
	QString ExecuteTypeToString(ExecuteType type)
	{
		switch (type)
		{
		case SqlLiteIDE::Query:
			return "返回查询结果";
			break;
		case SqlLiteIDE::NonQuery:
			return "返回受影响行数";
			break;
		default:
			return "";
			break;
		}
	}
	Ui::SqlLiteIDEClass ui;
	QsciLexerSQL* sqlLexer = new QsciLexerSQL();
	QTableView* view = new QTableView();
	QPlainTextEdit* executeText = new QPlainTextEdit();
	bool addDataBase(QString& fileName);
	void loadUi();
	void ExecuteSql();
	QComboBox* dbsComboBox = new QComboBox();
	QComboBox* executeTypeComboBox = new QComboBox();
	QString lastSql{ "" };
};
