#ifndef RESOURCE_MINITOR_H
#define RESOURCE_MINITOR_H
#include <QObject>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QString>
#if defined(Q_OS_LINUX)
#include "sys/statfs.h"
#else
#pragma comment(lib,"Kernel32.lib")
#pragma comment(lib,"Psapi.lib")
#include <windows.h>
#include <tlhelp32.h>
#include<direct.h>
#include<winternl.h>
#include <psapi.h>
//#include <atlconv.h>
#include <cmath>
#include <string.h>
#endif

class SystemInfo : public QObject
{
	Q_OBJECT
public:
	explicit SystemInfo(QObject* parent = nullptr);

public slots:
	void GetResource();
signals:
	void cpuRateChanged(double nCpuRate);
	void memRateChanged(double nMemRate);
	void diskRateChanged(double nDiskRate);
public:

	/**
	 * 获取内存使用情况
	 * @param nMemTotal 输出参数，总内存大小
	 * @param nMemUsed 输出参数，已使用的内存大小
	 * @return true表示成功获取内存使用情况，false表示获取失败
	 */
	bool GetMemUsage(double& nMemTotal, double& nMemUsed);

	/**
	 * 获取网络使用情况
	 * @return true表示成功获取网络使用情况，false表示获取失败
	 */
	bool GetNetUsage();

	/**
	 * 获取磁盘速度
	 * @return true表示成功获取磁盘速度，false表示获取失败
	 */
	bool GetDiskSpeed();

	/**
	 * 获取CPU使用率
	 * @param nCpuRate 输出参数，CPU使用率
	 * @return true表示成功获取CPU使用率，false表示获取失败
	 */
	bool GetCpuUsage(double& nCpuRate);

	/**
	 * 获取磁盘空间
	 * @param lFreeAll 输出参数，总空闲空间
	 * @param lTotalAll 输出参数，总空间大小
	 * @return true表示成功获取磁盘空间，false表示获取失败
	 */
	bool GetdiskSpace(unsigned long& lFreeAll, unsigned long& lTotalAll);

	/**
	 * 获取指定路径的空间
	 * @param path 指定的路径
	 * @return true表示成功获取指定路径的空间，false表示获取失败
	 */
	bool GetPathSpace(const QString& path);
private:
	const int m_timer_interval__ = 1000;
	QTimer monitor_timer__;
	double m_send_bytes__ = 0;
	double m_recv_bytes__ = 0;
	double m_disk_read__ = 0;
	double m_disk_write__ = 0;
	double m_cpu_total__ = 0;
	double m_cpu_use__ = 0;
};
#endif // RESOURCE_MINITOR_H
