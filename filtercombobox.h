#pragma once

#include <QComboBox>
#include <qcompleter.h>
#include <qlineedit.h>
#include <qstringlistmodel.h>
#include <qstring.h>

class FilterComboBox : public QComboBox
{
	Q_OBJECT

public:
	FilterComboBox(QWidget* parent = nullptr);
	~FilterComboBox();
	QCompleter* completer = new QCompleter(new QStringListModel(this));
	void setItems(const QStringList& items);
	void addItem(const QIcon& aicon, const QString& atext, const QVariant& auserData = QVariant());
	void addItem(const QString& atext, const QVariant& auserData = QVariant());
	void addCompleter(const QString& text) const;
};
