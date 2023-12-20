#pragma once

#include <QRunnable>
#include <QThreadPool>
#include <QMutex>
#include "constants.h"
#include <QRandomGenerator>
#include <QAtomicInt>
class RandomStringTask : public QObject, public QRunnable
{
	Q_OBJECT
public:
	// 构造函数，初始化生成随机字符串的任务
	RandomStringTask(int length, QString* result, QMutex* mutex, int offset, QAtomicInt* atomicInt, QList<int> type);
	// 析构函数
	~RandomStringTask();
	// 重写QRunnable的run方法，这里执行任务
	void run() override;
signals:
	// 进度信号，用于通知进度变化
	void progress(int value);
private:
	int m_length;               // 字符串的长度
	QString* m_result;          // 指向结果字符串的指针
	QMutex* m_mutex;            // 用于互斥访问的互斥锁
	int m_offset;               // 结果字符串的偏移量
	QList<int> m_type;          // 字符串类型的列表，可能用于定义生成字符串的规则
	QAtomicInt* m_atomicInt;    // 原子操作的整型，用于多线程中安全操作
};