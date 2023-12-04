#pragma once

#include <QtWidgets/QWidget>
#include <qsplitter.h>
#include <qstackedlayout.h>
#include <QLabel>

#include "iconlabellistview.h"
#include "navigatorview.h"
#include "jsontoyaml.h"
#include "timestamp.h"
#include "convertbinary.h"
#include "converthtml.h"
#include "converturl.h"
#include "convertbase64.h"
#include "convertbase64image.h"



class DevToys : public QWidget
{
    Q_OBJECT

public:
    DevToys(QWidget *parent = nullptr);
    ~DevToys();

private slots:
    /**
	 * @brief 根据objectName找到对应的widget并切换
	 * 这里的name是widget的objectName
    */
    void showToolWidget(QString name);
private:
    void loadUi();

    void loadConnect();
    IconLabelListView* listView = new IconLabelListView();
    NavigatorView* navigator = new NavigatorView();
    QSplitter* splitter = new QSplitter();
    QStackedLayout* stackedLayout = new QStackedLayout();


	QLabel* label = new QLabel();
	JsonToYaml* jsonToYaml = new JsonToYaml();
	Timestamp* timestamp = new Timestamp();
    ConvertBinary* convertBinary = new ConvertBinary();
	ConvertHtml* convertHtml = new ConvertHtml();
	ConvertUrl* convertUrl = new ConvertUrl();
	ConvertBase64* convertBase64 = new ConvertBase64();
	ConvertBase64Image* convertBase64Image = new ConvertBase64Image();
};
