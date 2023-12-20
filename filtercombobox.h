#pragma once

#include <QComboBox>
#include <qcompleter.h>
#include <qlineedit.h>
#include <qstringlistmodel.h>

class FilterComboBox : public QComboBox
{
	Q_OBJECT

public:
	FilterComboBox(QWidget* parent = nullptr);
	~FilterComboBox();
	QCompleter* completer = new QCompleter(this);

	void setItems(const QStringList& items);
};
