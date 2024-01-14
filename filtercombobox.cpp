#include "filtercombobox.h"

FilterComboBox::FilterComboBox(QWidget* parent)
	: QComboBox(parent)
{
	this->init();
}

FilterComboBox::FilterComboBox(const QStringList& items, QWidget* parent)
	:QComboBox(parent)
{
	this->init();
	this->addItems(items);
}

FilterComboBox::FilterComboBox(const QMap<QString, QString> textIcons, QWidget* parent)
{
	this->init();

	for (const QPair<QString, QString>& textIcon : textIcons.toStdMap())
	{
		this->addItem(QIcon(textIcon.second), textIcon.first);
	}
}

FilterComboBox::~FilterComboBox()
{}

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

void FilterComboBox::addItems(const QStringList& texts)
{
	QComboBox::addItems(texts);
	this->addCompleters(texts);
}

void FilterComboBox::addCompleter(const QString& text) const
{
	QStringListModel* model = (QStringListModel*)this->completer->model();
	model->insertRow(model->rowCount());
	model->setData(model->index(model->rowCount() - 1, 0), text);
}

void FilterComboBox::addCompleters(const QStringList& texts) const
{
	QStringListModel* model = (QStringListModel*)this->completer->model();
	for (const QString& text : texts)
	{
		model->insertRow(model->rowCount());
		model->setData(model->index(model->rowCount() - 1, 0), text);
	}
}

void FilterComboBox::init()
{
	this->setEditable(true);
	this->setCompleter(this->completer);
	// 不区分大小写
	this->completer->setFilterMode(Qt::MatchContains);
	this->lineEdit()->setCompleter(this->completer);
	/*这里是为了当输入的信息不在里面的时候不再显示任何item的图标，将当前的index置为-1*/
	connect(this->lineEdit(), &QLineEdit::textChanged, [=](const QString text)
		{
			auto model = qobject_cast<QStringListModel*>(this->completer->model());
			QStringList list = model->stringList();
			if (!list.contains(text) && this->currentIndex() >= 0)
			{
				this->setCurrentIndex(-1);
				/*这里按原样赋值并不会重复触发textChanged*/
				this->lineEdit()->setText(text);
			}
		});
}