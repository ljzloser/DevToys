#include "filtercombobox.h"

FilterComboBox::FilterComboBox(QWidget* parent)
	: QComboBox(parent)
{
	this->setEditable(true);
	this->setCompleter(this->completer);
	// 不区分大小写
	this->completer->setFilterMode(Qt::MatchContains);
	this->lineEdit()->setCompleter(this->completer);
	/*这里是为了当输入的信息不在里面的时候不再显示任何item的图标，将当前的index置为-1*/
	connect(this->lineEdit(), &QLineEdit::textChanged, [=](const QString text)
		{
			auto model = qobject_cast<QStringListModel*>(this->completer->completionModel());
			QStringList list = model->stringList();
			if (!list.contains(text) && this->currentIndex() >= 0)
			{
				this->setCurrentIndex(-1);
				/*这里按原样赋值并不会重复触发textChanged*/
				this->lineEdit()->setText(text);
			}
		});
}

FilterComboBox::~FilterComboBox()
{}

void FilterComboBox::setItems(const QStringList& items)
{
	this->completer->setModel(new QStringListModel(items));
	this->addItems(items);
	this->lineEdit()->setText("");
	this->setCurrentIndex(-1);
}

void FilterComboBox::addItem(const QIcon& aicon, const QString& atext, const QVariant& auserData)
{
	QComboBox::addItem(aicon, atext, auserData);
	this->addCompleter(atext);
}
void FilterComboBox::addItem(const QString& atext, const QVariant& auserData)
{
	QComboBox::addItem(atext, auserData);
	this->addCompleter(atext);
}

void FilterComboBox::addCompleter(const QString& text) const
{
	QStringListModel* model = (QStringListModel*)this->completer->model();
	model->insertRow(model->rowCount());
	model->setData(model->index(model->rowCount() - 1, 0), text);
}