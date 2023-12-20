#pragma once

#include <QtWidgets/QWidget>
#include <qsplitter.h>
#include <qstackedlayout.h>
#include <QLabel>
#include "qcustommainwindow.h"
#include "qcustomtitlebar.h"
#include "iconlabellistview.h"
#include "navigatorview.h"
#include "jsontoyaml.h"
#include "timestamp.h"
#include "convertbinary.h"
#include "converthtml.h"
#include "converturl.h"
#include "convertbase64.h"
#include "convertbase64image.h"
#include "jsonstringformat.h"
#include "xmlstringformat.h"
#include "passwordgeneration.h"
#include "stringhashgeneration.h"
#include "uuidgeneration.h"
#include "settingwidget.h"
#include "escapestringwidget.h"
#include "markdownpreviewwidget.h"
#include <qevent.h>

class DevToys : public QWidget
{
	Q_OBJECT

public:
	DevToys(QWidget* parent = nullptr);
	~DevToys();
	SettingWidget* settingWidget = new SettingWidget();
	QSplitter* splitter = new QSplitter();
public slots:
	void onFiterComboBoxTextChanged(QString text);

private slots:
	/**
	 * @brief 根据objectName找到对应的widget并切换
	 * 这里的name是widget的objectName
	*/
	void showToolWidget(QString name);
	void onParentItemClicked(QStringList& names);

private:
	void loadUi();
	void loadConnect();

	IconLabelListView* listView = new IconLabelListView();
	NavigatorView* navigator = new NavigatorView();

	QStackedLayout* stackedLayout = new QStackedLayout();

	QLabel* label = new QLabel();
	JsonToYaml* jsonToYaml = new JsonToYaml();
	Timestamp* timestamp = new Timestamp();
	ConvertBinary* convertBinary = new ConvertBinary();
	ConvertHtml* convertHtml = new ConvertHtml();
	ConvertUrl* convertUrl = new ConvertUrl();
	ConvertBase64* convertBase64 = new ConvertBase64();
	ConvertBase64Image* convertBase64Image = new ConvertBase64Image();
	JsonStringFormat* jsonStringFormat = new JsonStringFormat();
	XmlStringFormat* xmlStringFormat = new XmlStringFormat();
	StringHashGeneration* stringHashGeneration = new StringHashGeneration();
	PasswordGeneration* passwordGeneration = new PasswordGeneration();
	UuidGeneration* uuidGeneration = new UuidGeneration();
	EscapeStringWidget* escapeStringWidget = new EscapeStringWidget();
	MarkDownPreviewWidget* markDownPreviewWidget = new MarkDownPreviewWidget();
};
