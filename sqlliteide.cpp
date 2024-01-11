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
		SqlExecutor sqlExecutor(this->dbsComboBox->currentText());
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
	QStringList dbs = Config::getValue("dbs").toString().split(",");
	for (QString& db : dbs)
	{
		this->addDataBase(db);
	}
	this->loadBaseApis();
}

SqlLiteIDE::~SqlLiteIDE()
{
	QStringList dbs;

	for (int i = 0; i < this->dbsComboBox->count(); i++)
	{
		dbs.append(this->dbsComboBox->itemText(i));
	}
	Config::setValue("dbs", dbs.join(","));
}

bool SqlLiteIDE::addDataBase(QString& fileName)
{
	if (this->dbsComboBox->findText(fileName) == 1 || fileName.isEmpty())
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
	this->dbsComboBox->addItem(fileName);
	// Calculate the width of the widest item.
	// 计算 最宽的 item
	QFontMetrics fm(dbsComboBox->font());
	int widestItemWidth = 0;
	for (int i = 0; i < dbsComboBox->count(); ++i) {
		widestItemWidth = qMax(widestItemWidth, fm.horizontalAdvance(dbsComboBox->itemText(i)));
	}

	// Calculate extra width for margins, frame and dropdown arrow.
	// 计算 边距，边框和下拉箭头
	int extraWidth = dbsComboBox->style()->pixelMetric(QStyle::PM_ComboBoxFrameWidth) * 2;
	extraWidth += dbsComboBox->style()->pixelMetric(QStyle::PM_ScrollBarExtent);
	extraWidth += 2 * dbsComboBox->style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;

	if (widestItemWidth + extraWidth > 300)
	{
		widestItemWidth = 300 - extraWidth;
	}

	dbsComboBox->setFixedWidth(widestItemWidth + extraWidth);

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
			for (int i = 0; i < this->dbsComboBox->count(); i++)
			{
				dbList.append(this->dbsComboBox->itemText(i));
			}
			this->dbsComboBox->clear();
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
			this->dbsComboBox->removeItem(this->dbsComboBox->findText(fileName));
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
	codeInfoMenuBarLayout->addWidget(this->dbsComboBox);
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
			SqlExecutor sqlExecutor(this->dbsComboBox->currentText());
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
	connect(this->dbsComboBox, &QComboBox::currentTextChanged, [=]()
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
	SqlExecutor sqlExceutor(this->dbsComboBox->currentText());
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