#pragma once

#include <qstring.h>
#include <qvariant.h>
#include <qapplication.h>
#include <qfile.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qiodevice.h>
#include <qiodevicebase.h>
#include <qvariant.h>

class Config
{
public:
	static QVariant getValue(QString& key);
	static QVariant getValue(QString key);
	static void setValue(QString key, QVariant value);
	static QVariantMap getConfig();
private:
	/**
	 * @brief 如果没有配置表则创建
	*/
	static void createTable();
};
