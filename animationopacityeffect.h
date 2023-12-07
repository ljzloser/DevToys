#pragma once

#include <QGraphicsEffect>

class AnimationOpacityEffect  : public QGraphicsOpacityEffect
{
	Q_OBJECT
signals:
	void inAnimationFinished();
	void outAnimationFinished();

public:
	AnimationOpacityEffect(QObject *parent);
	~AnimationOpacityEffect();
	void setDuration(int duration) { this->m_duration = duration; };
	virtual void inAnimationStart();
	virtual void outAnimationStart();
private:

	int m_duration = 1000;
};
