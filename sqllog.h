#pragma once

#include <QObject>
#include "SqlExecutor.h"
#include <qdatetime.h>
#include <QApplication>

class SqlLog  : public QObject
{
	Q_OBJECT

public:
	SqlLog(QObject *parent);
	~SqlLog();
	static QList<QVariantMap> readLog(QDate BeginDate,QDate endDate);
	static void saveLog(const QString content);
	static int getLogCount();
	static QList<QDateTime> getLogDate();
	static QVariantMap getLog(int index);
private:
	static void createTable();
};
