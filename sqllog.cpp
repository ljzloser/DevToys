﻿#include "sqllog.h"

SqlLog::SqlLog(QObject *parent)
	: QObject(parent)
{}

SqlLog::~SqlLog()
{}

QList<QVariantMap> SqlLog::readLog(QDate BeginDate, QDate endDate)
{
	SqlLog::createTable();
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\log.db");
	QString sql = QString("SELECT id as '序号',datetime as '时间', content as '日志' FROM log WHERE datetime BETWEEN '%1' AND '%2'").arg(BeginDate.toString("yyyy-MM-dd"), endDate.toString("yyyy-MM-dd"));
	return sqlExecutor.executeQuery(sql);
}


void SqlLog::saveLog(const QString content)
{
	SqlLog::createTable();
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\log.db");
	QString sql = QString("INSERT INTO log (datetime, content) VALUES ('%1', '%2');").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"), content);
	sqlExecutor.executeNonQuery(sql);
}

void SqlLog::createTable()
{
	QString sql = "CREATE TABLE IF NOT EXISTS log ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"datetime DATETIME,"
		"content TEXT"
		");";
	SqlExecutor sqlExecutor(QApplication::applicationDirPath() + "\\log.db");
	sqlExecutor.executeNonQuery(sql);
}

