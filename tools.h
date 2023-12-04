﻿#pragma once
#include "json/json.h"
#include <yaml-cpp/yaml.h>
#include "constants.h"
#include <qstring.h>
#include <QApplication>
#include <qclipboard.h>
#include "popupmessage.h"
#include <qwidget.h>
#include "qrect.h"
#include <qcursor.h>
#include <qregularexpression.h>

/**
 * @brief 全局需要的静态函数的工具类
*/
class Tools
{
public:
	/**
	 * @brief 将YAML转换为JSON
	 * @param ynode  要转换的YAML
	 * @param jnode  转换后的JSON
	 * @return  转换是否成功
	*/
	static bool convertYamlToJson(const YAML::Node& ynode, Json::Value& jnode);
	/**
	 * @brief 将JSON转换为YAML
	 * @param jnode 要转换的JSON
	 * @param ynode 转换后的YAML
	 * @return 转换是否成功
	*/
	static bool convertJsonToYaml(const Json::Value& jnode, YAML::Node& ynode);
	/**
	 * @brief 格式化Icon资源路径
	 * @param name  要格式化的文件名
	 * @return 格式化后的路径
	*/
	static 	QString formatIconPath(QString name);
	/**
	 * @brief 格式化JSON
	 * @param jnode 需要格式化的JSON
	 * @param indent 缩进空格数
	 * @return 格式化后的JSON
	*/
	static QString formatJson(const Json::Value& jnode, int indent = 4);
	/**
	 * @brief 格式化YAML
	 * @param ynode 需要格式化的YAML
	 * @param indent 缩进空格数
	 * @return 格式化后的YAML
	*/
	static QString formatYaml(const YAML::Node& ynode, int indent = 4);
	/**
	 * @brief 向剪切板设置文本
	 * @param text
	*/
	static void setClipboard(QString text);
	/**
	 * @brief 获取剪切板的文本
	 * @return 剪切板的文本
	*/
	static QString getClipboard();
	/**
	 * @brief 向剪切板设置图片
	 * @param pixmap 
	*/
	static void setClipboardPixmap(QPixmap pixmap);
	/**
	 * @brief 获取剪切板的图片
	 * @return 
	*/
	static QPixmap getClipboardPixmap();
	/**
	 * @brief 弹出消息
	 * @param content
	*/
	static void showPopupMessage(QString content);
	/**
	 * @brief 将数字转换为指定进制的字符串
	 * @param number 要转换的数字
	 * @param currentBase 当前进制
	 * @param targetBase 目标进制
	 * @param formatOutput 是否格式化输出
	*/
	static QString convertBase(const QString& number, int currentBase, int targetBase,bool formatOutput);

	//编码HTML
	static QString htmlEncode(const QString& text);
	//解码HTML
	static QString htmlDecode(const QString& text);
	//URL编码
	static QString urlEncode(const QString& plainText);
	//URL解码
	static QString urlDecode(const QString& encodedText);
	//Base64编码
	static QString Base64Encode(const QString& inputString);
	//Base64解码
	static QString Base64Decode(const QString& base64String);

};

