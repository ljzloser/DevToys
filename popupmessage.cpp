#include "popupmessage.h"

PopupMessage::PopupMessage(QWidget* parent)
	: QLabel(parent)
{
	// 设置窗口透明度效果
	this->setGraphicsEffect(opacityEffect);
	this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	this->opacityEffect->inAnimationStart();
	this->opacityEffect->setDuration(300);
	connect(this->opacityEffect, &AnimationOpacityEffect::outAnimationFinished, this, &PopupMessage::close);
	this->setStyleSheet("background-color: rgb(50, 150, 150);color: rgb(232, 232, 0); border-radius: 10px;border: 2px solid black;");
}

PopupMessage::~PopupMessage()
{}

void PopupMessage::showEvent(QShowEvent* event)
{
	QTimer::singleShot(this->m_showTime, [=]() {this->opacityEffect->outAnimationStart(); });
}