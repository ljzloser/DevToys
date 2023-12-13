#include "filtercombobox.h"

FilterComboBox::FilterComboBox(QWidget *parent)
	: QComboBox(parent)
{
	this->setEditable(true);
	this->setCompleter(this->completer);
	this->completer->setFilterMode(Qt::MatchContains);
	this->lineEdit()->setCompleter(this->completer);
	// 设置提示文本
	this->lineEdit()->setPlaceholderText("输入想要使用的工具");

}

FilterComboBox::~FilterComboBox()
{}

void FilterComboBox::setItems(const QStringList & items)
{
	this->completer->setModel(new QStringListModel(items));
	this->addItems(items);
	this->setCurrentText("");
}
