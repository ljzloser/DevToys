#include "devtoys.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qtranslator.h>
#include "MainWindow.h"
#include <cstdlib>
#include "Config.h"
#include <qcolordialog.h>
#include "SystemInfo.h"

int main(int argc, char* argv[])
{
	_putenv_s("QT_FONT_DPI", "96");
	QApplication a(argc, argv);
	// 加载样式表文件

	QTranslator translator;
	if (translator.load(QApplication::applicationDirPath() + "\\translations\\qt_zh_CN.qm")) {
		a.installTranslator(&translator);
	}
	//	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // 启用高DPI缩放
	MainWindow w;
	//SqlLiteIDE w;
	// 获取当前屏幕大小
	QRect screenRect = QGuiApplication::primaryScreen()->geometry();
	QRect rect(0, 0, 1280, 720);
	rect.moveCenter(screenRect.center());
	w.setGeometry(rect);
	w.show();
	qDebug() << "程序启动";
	return a.exec();
}