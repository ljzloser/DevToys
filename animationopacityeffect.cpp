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
	fadeInAnimation->setDuration(this->m_duration);  
	fadeInAnimation->setStartValue(0.0);  
	fadeInAnimation->setEndValue(1.0);    
	fadeInAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	connect(fadeInAnimation, &QPropertyAnimation::finished, [=]() {emit this->inAnimationFinished(); });
}

void AnimationOpacityEffect::outAnimationStart()
{
	QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(this, "opacity");
	fadeOutAnimation->setDuration(this->m_duration);  
	fadeOutAnimation->setStartValue(1.0);  
	fadeOutAnimation->setEndValue(0.0);    
	fadeOutAnimation->start(QPropertyAnimation::DeleteWhenStopped);
	connect(fadeOutAnimation, &QPropertyAnimation::finished, [=]() {emit this->outAnimationFinished(); });
}
