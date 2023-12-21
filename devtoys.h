#pragma once

#include "convertbase64.h"
#include "convertbase64image.h"
#include "convertbinary.h"
#include "converthtml.h"
#include "converturl.h"
#include "escapestringwidget.h"
#include "iconlabellistview.h"
#include "jsonstringformat.h"
#include "jsontoyaml.h"
#include "markdownpreviewwidget.h"
#include "navigatorview.h"
#include "passwordgeneration.h"
#include "settingwidget.h"
#include "stringhashgeneration.h"
#include "timestamp.h"
#include "uuidgeneration.h"
#include "xmlstringformat.h"
#include <qcontainerfwd.h>
#include <qlabel.h>
#include <qsplitter.h>
#include <qstackedlayout.h>
#include <qstring.h>
#include <qtmetamacros.h>
#include <qwidget.h>
#include "GaugeBoardWidget/gaugeboardwidget.h"
#include "systeminfowidget.h"
#include "sqlliteide.h"

class DevToys : public QWidget
{
	Q_OBJECT

public:
	DevToys(QWidget* parent = nullptr);
	~DevToys();
	SettingWidget* settingWidget = new SettingWidget();
	QSplitter* splitter = new QSplitter();
public slots:
	/**
	 * 根据输入文本过滤树视图，通过选择并滚动到第一个匹配的项。
	 * 如果找不到匹配项，则显示相应的工具小部件。
	 *
	 * @param text 用于过滤树视图的文本。
	 */
	void onFiterComboBoxTextChanged(QString text);

private slots:

	void showToolWidget(QString name);
	void onParentItemClicked(QStringList& names);

private:
	void loadUi();
	void loadConnect();
	void addWidgetToLayout(QString name, QWidget* widget);

	IconLabelListView* listView = new IconLabelListView();
	NavigatorView* navigator = new NavigatorView();

	QStackedLayout* stackedLayout = new QStackedLayout();
	GaugeBoardWidget* gaugeBoardWidget = new GaugeBoardWidget(0, 180);
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
	SqlLiteIDE* sqlLiteIDE = new SqlLiteIDE();

	SystemInfoWidget* systemInfoWidget = new SystemInfoWidget();
};
