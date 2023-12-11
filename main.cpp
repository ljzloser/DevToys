#include "devtoys.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qtranslator.h>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载样式表文件
	QFile file(":/qdarkstyle/dark/darkstyle.qss");
	file.open(QFile::ReadOnly);
	if (file.isOpen())
	{
		QString styleSheet = QLatin1String(file.readAll());
		a.setStyleSheet(styleSheet);
	}
	QTranslator translator;
	if (translator.load(QApplication::applicationDirPath() + "\\translations\\qt_zh_CN.qm")) {
		a.installTranslator(&translator);
	}

	MainWindow w;
	// 获取当前屏幕大小
	QRect screenRect = QGuiApplication::primaryScreen()->geometry();
	QRect rect(0, 0, 1280, 800);
	rect.moveCenter(screenRect.center());
	w.setGeometry(rect);

    w.show();
    return a.exec();
}
