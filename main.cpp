#include "devtoys.h"
#include <QtWidgets/QApplication>
#include <qfile.h>

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


    DevToys w;
	w.resize(1280, 800);
    w.show();
    return a.exec();
}
