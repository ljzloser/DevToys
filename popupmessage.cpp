#include "popupmessage.h"

PopupMessage::PopupMessage(QWidget *parent)
	: QLabel(parent)
{
	this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	this->setStyleSheet("background-color: rgb(50, 150, 150);color: rgb(232, 232, 0); border-radius: 10px;border: 2px solid black;");
}

PopupMessage::~PopupMessage()
{}

void PopupMessage::showEvent(QShowEvent * event)
{
	QTimer::singleShot(1000, this, SLOT(close()));
}
