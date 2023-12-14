#include "Config.h"


QVariant Config::getValue(QString& key)
{
	Config::createConfigFile();

	//读取配置文件中的XML
	QString path = QApplication::applicationDirPath() + "/config.json";
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return QVariant();
	}
	else
	{
		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		QJsonObject obj = doc.object();
		if (!obj.contains(key))
		{
			return QVariant();
		}
		QVariant value = obj.value(key).toVariant();
		return value;
	}

}

void Config::setValue(QString key, QVariant value)
{
	Config::createConfigFile();
	QString path = QApplication::applicationDirPath() + "/config.json";

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	else
	{
		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		QJsonObject obj = doc.object();
		obj.insert(key, value.toJsonValue());
		file.close();
		file.open(QIODevice::WriteOnly);
		file.write(QJsonDocument(obj).toJson());
		file.close();
	}
}

QJsonObject Config::getConfig()
{
	Config::createConfigFile();

	//读取配置文件中的XML
	QString path = QApplication::applicationDirPath() + "/config.json";
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
	{
		return QJsonObject();
	}
	else
	{
		QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
		return doc.object();
	}
}


void Config::createConfigFile()
{
	QString path = QApplication::applicationDirPath() + "/config.json";
	if (!QFile::exists(path))
	{
		QFile file(path);
		file.open(QIODevice::WriteOnly);
		file.close();
	}

}
