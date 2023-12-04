#include "iconlabel.h"
#include <qmessagebox.h>
#include "Qsci/qsciscintilla.h"

IconLabel::IconLabel(QWidget* parent)
	: QFrame(parent)
{
	ui.setupUi(this);
	this->setStyleSheet("background-color: rgb(54, 71, 96);");
	//设置换行
	ui.descriptionLabel->setWordWrap(true);
	QWidget* widget = new QWidget();
}

IconLabel::~IconLabel()
{
}

void IconLabel::setIcon(QPixmap icon)
{

	ui.imageLabel->setPixmap(icon.scaled(ui.imageLabel->size()));
}

void IconLabel::setTitle(QString title)
{
	ui.titleLabel->setText(title);
}

void IconLabel::setDescriptions(QString descriptions)
{
	ui.descriptionLabel->setText(descriptions);
}

void IconLabel::enterEvent(QEnterEvent* event)
{
	this->setStyleSheet("background-color: rgb(101, 129, 174);");
}

void IconLabel::leaveEvent(QEvent* event)
{
	this->setStyleSheet("background-color: rgb(54, 71, 96);");
}

void IconLabel::mousePressEvent(QMouseEvent* event)
{
	//如果是左键
	if (event->button() == Qt::LeftButton)
	{
		emit clicked(ui.titleLabel->text());
	}
}
