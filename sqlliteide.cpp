#include "sqlliteide.h"
#include <QPushButton>
#include <QFileDialog>
#include <qmessagebox.h>
#include <qstring.h>
#include <qtableview.h>

#include <QSCi/qsciapis.h>
#include <Qsci/qsciscintilla.h>

#include "Config.h"

static QStringList sqliteKeywords() {
	QStringList keywords;
	keywords << "ABORT" << "ACTION" << "ADD" << "AFTER" << "ALL" << "ALTER" << "ANALYZE"
		<< "AND" << "AS" << "ASC" << "ATTACH" << "AUTOINCREMENT" << "BEFORE" << "BEGIN"
		<< "BETWEEN" << "BY" << "CASCADE" << "CASE" << "CAST" << "CHECK" << "COLLATE"
		<< "COLUMN" << "COMMIT" << "CONFLICT" << "CONSTRAINT" << "CREATE" << "CROSS"
		<< "CURRENT_DATE" << "CURRENT_TIME" << "CURRENT_TIMESTAMP" << "DATABASE"
		<< "DEFAULT" << "DEFERRABLE" << "DEFERRED" << "DELETE" << "DESC" << "DETACH"
		<< "DISTINCT" << "DROP" << "EACH" << "ELSE" << "END" << "ESCAPE" << "EXCEPT"
		<< "EXCLUSIVE" << "EXISTS" << "EXPLAIN" << "FAIL" << "FOR" << "FOREIGN"
		<< "FROM" << "FULL" << "GLOB" << "GROUP" << "HAVING" << "IF" << "IGNORE"
		<< "IMMEDIATE" << "IN" << "INDEX" << "INDEXED" << "INITIALLY" << "INNER" << "INSERT"
		<< "INSTEAD" << "INTERSECT" << "INTO" << "IS" << "ISNULL" << "JOIN" << "KEY"
		<< "LEFT" << "LIKE" << "LIMIT" << "MATCH" << "NATURAL" << "NO" << "NOT"
		<< "NOTNULL" << "NULL" << "OF" << "OFFSET" << "ON" << "OR" << "ORDER"
		<< "OUTER" << "PLAN" << "PRAGMA" << "PRIMARY" << "QUERY" << "RAISE"
		<< "RECURSIVE" << "REFERENCES" << "REGEXP" << "REINDEX" << "RELEASE"
		<< "RENAME" << "REPLACE" << "RESTRICT" << "RIGHT" << "ROLLBACK" << "ROW"
		<< "SAVEPOINT" << "SELECT" << "SET" << "TABLE" << "TEMP" << "TEMPORARY"
		<< "THEN" << "TO" << "TRANSACTION" << "TRIGGER" << "UNION" << "UNIQUE"
		<< "UPDATE" << "USING" << "VACUUM" << "VALUES" << "VIEW" << "VIRTUAL" << "WHEN"
		<< "WHERE" << "WITH" << "WITHOUT"
		<< "abart" << "action" << "add" << "after" << "all" << "alter" << "analyze"
		<< "and" << "as" << "asc" << "attach" << "autoincrement" << "before" << "begin"
		<< "between" << "by" << "cascade" << "case" << "cast" << "check" << "collate"
		<< "column" << "commit" << "conflict" << "constraint" << "create" << "cross"
		<< "current_date" << "current_time" << "current_timestamp" << "database"
		<< "default" << "deferrable" << "deferred" << "delete" << "desc" << "detach"
		<< "distinct" << "drop" << "each" << "else" << "end" << "escape" << "except"
		<< "exclusive" << "exists" << "explain" << "fail" << "for" << "foreign"
		<< "from" << "full" << "glob" << "group" << "having" << "if" << "ignore"
		<< "immediate" << "in" << "index" << "indexed" << "initially" << "inner" << "insert"
		<< "instead" << "intersect" << "into" << "is" << "isnull" << "join" << "key"
		<< "left" << "like" << "limit" << "match" << "natural" << "no" << "not"
		<< "notnull" << "null" << "of" << "offset" << "on" << "or" << "order"
		<< "outer" << "plan" << "pragma" << "primary" << "query" << "raise"
		<< "recursive" << "references" << "regexp" << "reindex" << "release"
		<< "rename" << "replace" << "restrict" << "right" << "rollback" << "row"
		<< "savepoint" << "select" << "set" << "table" << "temporary" << "then" << "to"
		<< "transaction" << "trigger" << "union" << "unique" << "update" << "using"
		<< "vacuum" << "values" << "view" << "virtual" << "when"
		<< "where" << "with" << "without";
	return keywords;
}

void SqlLiteIDE::loadBaseApis()
{
	QsciAPIs* baseapis = new QsciAPIs(this->sqlLexer);
	for (QString& keyword : sqliteKeywords())
	{
		baseapis->add(keyword);
	}
	if (!this->dbsComboBox->currentText().isEmpty())
	{
		SqlExecutor sqlExecutor(this->DbsComboBox()->currentText());
		QStringList tables = sqlExecutor.executeFirstColumn<QString>("SELECT name FROM sqlite_master");

		for (QString& table : tables)
		{
			baseapis->add(table);
		}
	}
	baseapis->prepare();
	sqlLexer->setAPIs(baseapis);
}

SqlLiteIDE::SqlLiteIDE(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadUi();

	ui.codeEdit->setLexer(this->sqlLexer);
	ui.codeEdit->setAutoCompletionCaseSensitivity(ui.codeEdit->caseSensitive()); // 补全的时候不区分大小写
	//QString name = QString("I:\\project\\C++\\Qt\\DevToys\\x64\\Debug\\database.db");
	//loadBaseApis();
	//this->addDataBase(name);
	QStringList dbs = Config::getValue("dbs").toString().split(";");
	for (QString& db : dbs)
	{
		this->addDataBase(db);
	}
	this->loadBaseApis();
}

SqlLiteIDE::~SqlLiteIDE()
{
	QStringList dbs;

	for (int i = 0; i < this->DbsComboBox()->count(); i++)
	{
		dbs.append(this->DbsComboBox()->itemText(i));
	}
	Config::setValue("dbs", dbs.join(";"));
}

bool SqlLiteIDE::addDataBase(QString& fileName)
{
	if (this->DbsComboBox()->findText(fileName) == 1 || fileName.isEmpty())
	{
		return true;
	}

	SqlExecutor sqlExecutor(fileName);
	// 如果打开失败
	if (!sqlExecutor.isValid())
	{
		QMessageBox::warning(this, tr("警告"), sqlExecutor.lastError());
		return false;
	}
	QTreeWidgetItem* dbItem = new QTreeWidgetItem();
	dbItem->setText(0, fileName);
	ui.dbInfoTreeWidget->addTopLevelItem(dbItem);

	QTreeWidgetItem* tableItem = new QTreeWidgetItem();
	tableItem->setText(0, "表");
	dbItem->addChild(tableItem);

	QStringList tables = sqlExecutor.executeFirstColumn<QString>(QString("SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY name;"));
	for (QString& table : tables)
	{
		QTreeWidgetItem* tableNameItem = new QTreeWidgetItem();
		tableNameItem->setText(0, table);
		tableItem->addChild(tableNameItem);
		QList<QVariantMap> columnMapList = sqlExecutor.executeQuery("PRAGMA table_info(" + table + ")");
		for (QVariantMap& tableInfoMap : columnMapList)
		{
			QTreeWidgetItem* columnItem = new QTreeWidgetItem();
			QStringList columnInfoList;
			if (tableInfoMap["pk"].toString() == "1")
			{
				columnInfoList << "PK" << tableInfoMap["type"].toString() << (tableInfoMap["notnull"].toString() == "1" ? "not null" : "null");
			}
			else
			{
				columnInfoList << tableInfoMap["type"].toString() << (tableInfoMap["notnull"].toString() == "1" ? "not null" : "null");
			}
			QString text = QString("%1(%2)").arg(tableInfoMap["name"].toString()).arg(columnInfoList.join(","));
			columnItem->setText(0, text);
			tableNameItem->addChild(columnItem);
		}
	}

	QTreeWidgetItem* viewItem = new QTreeWidgetItem();
	viewItem->setText(0, "视图");
	dbItem->addChild(viewItem);

	QStringList views = sqlExecutor.executeFirstColumn<QString>(QString("SELECT name FROM sqlite_master WHERE type = 'view' ORDER BY name;"));
	for (QString& view : views)
	{
		QTreeWidgetItem* viewNameItem = new QTreeWidgetItem();
		viewNameItem->setText(0, view);
		viewItem->addChild(viewNameItem);
	}
	this->DbsComboBox()->addItem(fileName);
	// Calculate the width of the widest item.
	// 计算 最宽的 item
	QFontMetrics fm(DbsComboBox()->font());
	int widestItemWidth = 0;
	for (int i = 0; i < DbsComboBox()->count(); ++i) {
		widestItemWidth = qMax(widestItemWidth, fm.horizontalAdvance(DbsComboBox()->itemText(i)));
	}

	// Calculate extra width for margins, frame and dropdown arrow.
	// 计算 边距，边框和下拉箭头
	int extraWidth = DbsComboBox()->style()->pixelMetric(QStyle::PM_ComboBoxFrameWidth) * 2;
	extraWidth += DbsComboBox()->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
	extraWidth += 2 * DbsComboBox()->style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

	if (widestItemWidth + extraWidth > 300)
	{
		widestItemWidth = 300 - extraWidth;
	}

	DbsComboBox()->setFixedWidth(widestItemWidth + extraWidth);

	return true;
}

void SqlLiteIDE::loadUi()
{
#pragma region 左边栏
	QHBoxLayout* dbInfoMenuBarLayout = new QHBoxLayout();
	dbInfoMenuBarLayout->setContentsMargins(0, 0, 0, 0);
	QPushButton* openButton = new QPushButton("打开");
	QPushButton* refreshButton = new QPushButton("刷新");
	QPushButton* deleteButton = new QPushButton("删除");
	QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
	dbInfoMenuBarLayout->addWidget(openButton);
	dbInfoMenuBarLayout->addWidget(refreshButton);
	dbInfoMenuBarLayout->addWidget(deleteButton);
	dbInfoMenuBarLayout->addItem(horizontalSpacer);
	ui.dbInfoMenuBar->setLayout(dbInfoMenuBarLayout);

	ui.dbInfoTreeWidget->setHeaderHidden(true);
	ui.dbInfoTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.dbInfoTreeWidget, &QTreeWidget::customContextMenuRequested, this, &SqlLiteIDE::treeMenuShow);

	connect(openButton, &QPushButton::clicked, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("选择SqlLite数据库"), "", tr("SQLite Database Files(*.sqlite * .db);; All Files(*)"));
			if (fileName.isEmpty() || !this->addDataBase(fileName))
			{
				return;
			}
		});
	connect(refreshButton, &QPushButton::clicked, [=]()
		{
			QStringList dbList;
			for (int i = 0; i < this->DbsComboBox()->count(); i++)
			{
				dbList.append(this->DbsComboBox()->itemText(i));
			}
			this->DbsComboBox()->clear();
			ui.dbInfoTreeWidget->clear();
			for (QString& dbName : dbList)
			{
				this->addDataBase(dbName);
			}
		});
	connect(deleteButton, &QPushButton::clicked, [=]()
		{
			// 获取树形控件选中的项
			QTreeWidgetItem* item = ui.dbInfoTreeWidget->currentItem();
			if (item == NULL)
			{
				return;
			}
			// 循环获取最顶层的项
			while (item->parent())
			{
				item = item->parent();
			}
			QString fileName = item->text(0);
			this->DbsComboBox()->removeItem(this->DbsComboBox()->findText(fileName));
			ui.dbInfoTreeWidget->takeTopLevelItem(ui.dbInfoTreeWidget->indexOfTopLevelItem(item));
		});
#pragma endregion

#pragma region 右边栏
	QSpacerItem* leftSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Maximum);
	QHBoxLayout* codeInfoMenuBarLayout = new QHBoxLayout();
	codeInfoMenuBarLayout->setContentsMargins(0, 0, 0, 0);

	QPushButton* executeButton = new QPushButton("执行");
	// 设置快捷键
	executeButton->setShortcut(Qt::Key_F5);

	QPushButton* saveResultButton = new QPushButton("保存结果");
	saveResultButton->setShortcut(Qt::CTRL + Qt::Key_S);

	this->executeTypeComboBox->addItem(ExecuteTypeToString(ExecuteType::Query), QVariant::fromValue(ExecuteType::Query));
	this->executeTypeComboBox->addItem(ExecuteTypeToString(ExecuteType::NonQuery), QVariant::fromValue(ExecuteType::NonQuery));
	QSpacerItem* horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
	codeInfoMenuBarLayout->addItem(leftSpacer);
	codeInfoMenuBarLayout->addWidget(this->DbsComboBox());
	codeInfoMenuBarLayout->addWidget(this->executeTypeComboBox);
	codeInfoMenuBarLayout->addWidget(executeButton);
	codeInfoMenuBarLayout->addWidget(saveResultButton);
	codeInfoMenuBarLayout->addItem(horizontalSpacer1);
	ui.codeMenuBar->setLayout(codeInfoMenuBarLayout);
	connect(executeButton, &QPushButton::clicked, [=]()
		{
			ExecuteSql();
		});
	connect(saveResultButton, &QPushButton::clicked, [=]()
		{
			if (this->lastSql.isEmpty())
			{
				return;
			}
			SqlExecutor sqlExecutor(this->DbsComboBox()->currentText());
			QString data = sqlExecutor.executeQueryCsv(this->lastSql);
			if (data.isEmpty())
			{
				return;
			}
			QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), "结果", tr("CSV文件(*.csv)"));
			if (!fileName.isEmpty())
			{
				// 打开文件
				QFile file(fileName);
				if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
				{
					QMessageBox::warning(this, tr("提示"), tr("保存失败"));
					return;
				}
				file.write("\xEF\xBB\xBF" + data.toUtf8());

				// 关闭文件

				file.close();
				QMessageBox::information(this, tr("提示"), QString("保存至%1").arg(fileName));
			}
		});
	connect(this->DbsComboBox(), &QComboBox::currentTextChanged, [=]()
		{
			this->loadBaseApis();
		});
#pragma endregion
}

void SqlLiteIDE::ExecuteSql()
{
	QString text = ui.codeEdit->selectedText();
	if (text.isEmpty())
	{
		text = ui.codeEdit->text();
	}
	SqlExecutor sqlExceutor(this->DbsComboBox()->currentText());
	ExecuteType type = (ExecuteType)this->executeTypeComboBox->currentData().toInt();
	try
	{
		switch (type)
		{
		case ExecuteType::Query:
		{
			QStandardItemModel* model = sqlExceutor.executeQueryModel(text);
			delete this->view->model();
			view->setModel(model);
			ui.dockWidget->setWidget(view);
			ui.dockWidget->show();
			break;
		}
		case ExecuteType::NonQuery:
		{
			int count = sqlExceutor.executeNonQuery(text);
			this->executeText->setPlainText(QString("影响行数:%1").arg(count));
			ui.dockWidget->setWidget(this->executeText);
			ui.dockWidget->show();
			break;
		}
		}
		lastSql = text;
	}
	catch (const std::exception& e)
	{
		this->executeText->setPlainText(QString("Error:%1").arg(e.what()));
		ui.dockWidget->setWidget(this->executeText);
		ui.dockWidget->show();
	}
}

void SqlLiteIDE::treeMenuShow(QPoint point)
{
	QTreeWidgetItem* item = ui.dbInfoTreeWidget->itemAt(point);
	if (item)
	{
		QTreeWidgetItem* parent = item->parent();
		if (parent)
		{
			QString text = parent->text(0);
			QMenu menu;
			if (text == "表")
			{
				QAction* createAction = menu.addAction("创建脚本");
				createAction->setData(QVariant(ScriptType::Create));
				QAction* updateAction = menu.addAction("更新脚本");
				updateAction->setData(QVariant(ScriptType::Update));
				QAction* deleteAction = menu.addAction("删除脚本");
				deleteAction->setData(QVariant(ScriptType::Delete));
				QAction* insertAction = menu.addAction("插入脚本");
				insertAction->setData(QVariant(ScriptType::Insert));
				QAction* selectAction = menu.addAction("查询脚本");
				selectAction->setData(QVariant(ScriptType::Select));
				// 分隔
				menu.addSeparator();
				QAction* viewDataAction = menu.addAction("查看数据");
				viewDataAction->setData(QVariant(ScriptType::ViewData));

				QAction* UpdateDataAction = menu.addAction("修改数据");
				UpdateDataAction->setData(QVariant(ScriptType::UpdateData));

				QAction* exportAction = menu.addAction("导出数据");
				exportAction->setData(QVariant(ScriptType::ExportData));
			}
			else if (text == "视图")
			{
				QAction* createAction = menu.addAction("创建脚本");
				createAction->setData(QVariant(ScriptType::Create));

				QAction* updateAction = menu.addAction("查询脚本");
				updateAction->setData(QVariant(ScriptType::Select));
			}
			connect(&menu, &QMenu::triggered, [=](QAction* action)
				{
					ScriptType scriptType = (ScriptType)action->data().toInt();
					createSqlScript(scriptType, item);
				});
			menu.exec(ui.dbInfoTreeWidget->viewport()->mapToGlobal(point));
		}
	}
}

void SqlLiteIDE::createSqlScript(ScriptType scriptType, QTreeWidgetItem* item)
{
	SqlExecutor sqlExecutor(this->DbsComboBox()->currentText());
	QString script = "";
	QString table = item->text(0);
	switch (scriptType)
	{
	case SqlLiteIDE::ScriptType::Create:
	{
		script = sqlExecutor.executeScalar<QString>(QString("SELECT sql FROM sqlite_master WHERE name = '%1'").arg(table));
		break;
	}
	case SqlLiteIDE::ScriptType::Update:
	{
		auto map = sqlExecutor.executeQuery("PRAGMA table_info(" + table + ")");
		QStringList columnList;
		for (auto& map : map)
		{
			QString column = map.value("name").toString();
			QString type = map.value("type").toString();
			if (type == "INTEGER" || type == "REAL" || type == "NUMERIC ")
			{
				columnList << ("    " + column + " = " + type);
			}
			else
			{
				columnList << ("    " + column + " = '" + type + "'");
			}
		}
		script = QString("UPDATE %1 SET \n%2\nWHERE 1 = 1").arg(table).arg(columnList.join(",\n"));

		break;
	}
	case SqlLiteIDE::ScriptType::Delete:
	{
		script = "DELETE FROM " + table + " WHERE 1 = 1";
		break;
	}
	case SqlLiteIDE::ScriptType::Insert:
	{
		auto map = sqlExecutor.executeQuery("PRAGMA table_info(" + table + ")");

		QStringList columnList;
		QStringList typeList;

		for (auto& map : map)
		{
			columnList << ("    " + map.value("name").toString());
			QString type = map.value("type").toString();
			if (type == "INTEGER" || type == "REAL" || type == "NUMERIC ")
			{
				typeList << "    " + type;
			}
			else
			{
				typeList << QString("    '%1'").arg(type);
			}
		}
		script = QString("INSERT INTO %1 (\n%2\n) VALUES (\n%3\n)").arg(table).arg(columnList.join(",\n")).arg(typeList.join(",\n"));

		break;
	}
	case SqlLiteIDE::ScriptType::Select:
	{
		auto map = sqlExecutor.executeQuery("PRAGMA table_info(" + table + ")");
		QStringList columnList;
		for (auto& map : map)
		{
			columnList << "    " + map.value("name").toString();
		}

		script = QString("SELECT\n%1\nFROM %2").arg(columnList.join(",\n")).arg(table);

		break;
	}
	case SqlLiteIDE::ScriptType::ViewData:
	{
		QTableView* view = getTableView(table);

		// 设置只读
		view->setEditTriggers(QAbstractItemView::NoEditTriggers);

		view->show();

		break;
	}
	case SqlLiteIDE::ScriptType::UpdateData:
	{
		QTableView* view = getTableView(table);
		QSqlTableModel* model = (QSqlTableModel*)view->model();
		//connect(model, &QSqlTableModel::be, [=]()
		//	{
		//		qDebug()<< "这里触发了"
		//	});

		break;
	}
	case SqlLiteIDE::ScriptType::ExportData:
	{
		break;
	}
	default:
		break;
	}
	ui.codeEdit->setText(script);
}

QTableView* SqlLiteIDE::getTableView(QString& table)
{
	QTableView* view = new QTableView();
	QSqlDatabase db = SqlExecutor(this->DbsComboBox()->currentText()).getDatabase();
	QSqlTableModel* model = new QSqlTableModel(view, db);
	model->setTable(table);
	model->select();
	view->setModel(model);
	view->resizeColumnsToContents();
	return view;
}