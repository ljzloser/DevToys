#include "Config.h"
#include "sqllog.h"

QVariant Config::getValue(QString& key)
{
	Config::createTable();
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\database.db");
	QString sql = QString("SELECT value FROM config WHERE key = '%1'").arg(key);
	return 	sqlExecutor.executeScalar<QVariant>(sql);
}
QVariant Config::getValue(QString key)
{
	Config::createTable();
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\database.db");
	QString sql = QString("SELECT value FROM config WHERE key = '%1'").arg(key);
	return 	sqlExecutor.executeScalar<QVariant>(sql);
}
void Config::setValue(QString key, QVariant value)
{
	Config::createTable();
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\database.db");
	QString sql = QString("SELECT COUNT(id) FROM config where Key = '%1'").arg(key);
	if (sqlExecutor.executeScalar(sql, 0) == 0)
	{
		sql = QString("insert into config(key,value) values('%1','%2')").arg(key).arg(value.toString());
	}
	else
	{
		sql = QString("update config set value = '%1' where key = '%2'").arg(value.toString()).arg(key);
	}
	sqlExecutor.executeNonQuery(sql);
	SqlLog::saveLog(QString("{%1,%2}").arg(key).arg(value.toString()));
}

QVariantMap Config::getConfig()
{
	Config::createTable();
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\database.db");
	QVariantMap result;
	for (QVariantMap& map : sqlExecutor.executeQuery("select * from config"))
	{
		result[map["key"].toString()] = map["value"];
	}
	return result;
}

void Config::createTable()
{
	QString sql = "CREATE TABLE IF NOT EXISTS config ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"key TEXT,"
		"value TEXT"
		");";
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\database.db");
	sqlExecutor.executeNonQuery(sql);
}
