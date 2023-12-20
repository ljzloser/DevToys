#include "animationopacityeffect.h"
#include <qpropertyanimation.h>

AnimationOpacityEffect::AnimationOpacityEffect(QObject* parent)
	: QGraphicsOpacityEffect(parent)
{
	this->setOpacity(0.0);
}

AnimationOpacityEffect::~AnimationOpacityEffect()
{}

void AnimationOpacityEffect::inAnimationStart()
{
	QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(this, "opacity");
	connect(fadeInAnimation, &QPropertyAnimation::finished, [=]() {emit this->inAnimationFinished(); });
	this->setAnimation(fadeInAnimation, 0.0, 1.0);
}

void AnimationOpacityEffect::setAnimation(QPropertyAnimation* animation, const QVariant& begin, const QVariant& end) const
{
	animation->setDuration(this->m_duration);
	animation->setStartValue(begin);
	animation->setEndValue(end);
	animation->start(QPropertyAnimation::DeleteWhenStopped);
}

void AnimationOpacityEffect::outAnimationStart()
{
	QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(this, "opacity");
	connect(fadeOutAnimation, &QPropertyAnimation::finished, [=]() {emit this->outAnimationFinished(); });
	this->setAnimation(fadeOutAnimation, 1.0, 0.0);
}