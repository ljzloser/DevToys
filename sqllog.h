#pragma once

#include <QObject>
#include "SqlExecutor.h"
#include <qdatetime.h>
#include <QApplication>
#include <qstring.h>

class SqlLog  : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief 读取日志
	 * @param BeginDate 开始日期
	 * @param endDate 结束日期
	 * @return 
	*/
	static QList<QVariantMap> readLog(QDate BeginDate,QDate endDate);
	/**
	 * @brief 保存日志，这里是进行缓存。
	 * 如果需要及时保存，传入isWrite = true，注意传入后会将已经缓存的一并写入
	 * @param content 日志内容
	 * @param isWrite 是否写入
	*/
	static void saveLog(const QString content,bool isWrite = false);
	static int getLogCount();
	static QList<QDateTime> getLogDate();
	static QVariantMap getLog(int index);
	/**
	 * @brief 将日志写入数据库
	*/
	static void writeLog();
	/**
	 * @brief 设置缓存数量,如果等于0就是10，如果是1相当于不缓存
	 * @param count	缓存数量
	*/
	static void setCacheCount(int count);

private:
	/**
	 * @brief 如果没有日志表则创建
	*/
	static void createTable();
	/**
	 * @brief 日志缓存
	*/
	static QList<QString> logs;
	// 缓存数量
	static int cacheCount;
};
