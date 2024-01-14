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
#include <qsqltablemodel.h>

class SqlLiteIDE : public QWidget
{
	Q_OBJECT
private:
	enum ExecuteType
	{
		Query, //select
		NonQuery, //insert, update, delete
	};
	// 脚本类型
	enum ScriptType
	{
		Create = 0, //创建
		Update = 1, //更新
		Delete = 2, //删除
		Insert = 3, //插入
		Select = 4, //查询
		ViewData = 5, //查看数据
		UpdateData = 6, //更新数据
		ExportData = 7, //导出数据
	};
public:
	void loadBaseApis();
	SqlLiteIDE(QWidget* parent = nullptr);
	~SqlLiteIDE();

	QComboBox* DbsComboBox() const { return dbsComboBox; }
	void DbsComboBox(QComboBox* val) { dbsComboBox = val; }
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
	void treeMenuShow(QPoint point);
	void createSqlScript(ScriptType scriptType, QTreeWidgetItem* item);
	QTableView* getTableView(QString& table);
};
