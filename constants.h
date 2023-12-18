#pragma once
// constants.h 文件
#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <map>
#include <QString>
#include <qlist.h>

const std::map<QString, QStringList> treeMap{
	{"转换类型工具",{"JSON-YAML数据类型互转工具","Unix时间戳转换工具","进制转换工具"}},
	{"编码解码类型工具",{"HTML编码解码工具","URL编码解码工具","Base64文本编码解码工具","Base64图像编码解码工具"}},
	{"格式化类型工具",{"JSON字符串格式化工具","可扩展标记语言XML格式化工具"}},
	{"生成类型工具",{"哈希散列生成工具","密码生成器","通用唯一识别码生成工具"}},
	{"文本处理类型工具",{"文本转义-反转义"}},
	{"图片处理类型工具",{}},
};
const std::map<QString, QString> descriptionsMap{
	{"JSON-YAML数据类型互转工具","JSON和YAML数据类型互转工具"},
	{"Unix时间戳转换工具","将时间戳转换为人类可读的日期，反之亦然"},
	{"进制转换工具","将数字从一个进制转换为另一个进制"},
	{"HTML编码解码工具","将所有适用的字符编码或解码为对应的HTML输出"},
	{"URL编码解码工具","将所有适用的字符编码或解码为对应的URL输出"},
	{"Base64文本编码解码工具","编码或解码Base64文本数据"},
	{"Base64图像编码解码工具","编码或解码Base64图像数据"},
	{"JSON字符串格式化工具","格式化或压缩JSON字符串数据"},
	{"可扩展标记语言XML格式化工具","格式化或压缩一个XML字符串"},
	{"哈希散列生成工具","从文本数据计算MD5,SHA1,SHA256和SHA512的哈希值"},
	{"密码生成器","生成随机密码"},
	{"通用唯一识别码生成工具","生成唯一识别码"},
	{"文本转义-反转义","转义或反转义一个字符串"},




	{"设置","对程序某些选项进行设置提供更好的体验"}
};
// Icon资源目录
const QString ICONDIR = ":/DevToys/icon/";
const QList<QString> typeLists = { "png", "jpeg", "jpg", "bmp", "gif","x-icon","webp","svg+xml", };
const std::map<int, QString>randomStringCharMap{
	{0,"abcdefghijklmnopqrstuvwxyz"},
	{1,"ABCDEFGHIJKLMNOPQRSTUVWXYZ"},
	{2,"0123456789"},
	{3,"!@#$%^&*"},
};
#endif //CONSTANTS_H