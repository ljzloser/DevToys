#pragma once

#include <QComboBox>
#include <qcompleter.h>
#include <qlineedit.h>
#include <qstring.h>
#include <qstringlistmodel.h>
#include <qvariant.h>

class FilterComboBox : public QComboBox
{
	Q_OBJECT

public:
	FilterComboBox(QWidget* parent = nullptr);
	FilterComboBox(const QStringList& texts, QWidget* parent = nullptr);
	FilterComboBox(const QMap<QString, QString> textIcons, QWidget* parent = nullptr);
	~FilterComboBox();
	void addItem(const QIcon& aicon, const QString& atext, const QVariant& auserData = QVariant());
	void addItem(const QString& atext, const QVariant& auserData = QVariant());
	void addItems(const QStringList& texts);
private:
	QCompleter* completer = new QCompleter(new QStringListModel(this));
	void addCompleter(const QString& text) const;
	void addCompleters(const QStringList& texts) const;
	void init();
};
