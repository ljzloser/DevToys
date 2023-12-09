#include "animationopacityeffect.h"
#include <qpropertyanimation.h>

AnimationOpacityEffect::AnimationOpacityEffect(QObject *parent)
	: QGraphicsOpacityEffect(parent)
{
	this->setOpacity(0.0);
}

AnimationOpacityEffect::~AnimationOpacityEffect()
{}

void AnimationOpacityEffect::inAnimationStart()
{
	QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(this, "opacity");
	fadeInAnimation->setDuration(this->m_duration);  // ����ʱ�� 1000 ����
	fadeInAnimation->setStartValue(0.0);  // ��ʼ͸����
	fadeInAnimation->setEndValue(1.0);    // ����͸����
	fadeInAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	connect(fadeInAnimation, &QPropertyAnimation::finished, [=]() {emit this->inAnimationFinished(); });
}

void AnimationOpacityEffect::outAnimationStart()
{
	QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(this, "opacity");
	fadeOutAnimation->setDuration(this->m_duration);  // ����ʱ�� 1000 ����
	fadeOutAnimation->setStartValue(1.0);  // ��ʼ͸����
	fadeOutAnimation->setEndValue(0.0);    // ����͸����
	fadeOutAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	connect(fadeOutAnimation, &QPropertyAnimation::finished, [=]() {emit this->outAnimationFinished(); });
}
