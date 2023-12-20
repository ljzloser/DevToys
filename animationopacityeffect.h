#pragma once

#include <QGraphicsEffect>
#include <QPropertyAnimation>

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
	/**
	 * @brief 设置动画的开始和结束
	 * @param animation 属性动画指针
	 * @param begin 开始
	 * @param end 结束
	*/
	virtual void setAnimation(QPropertyAnimation* animation, const QVariant& begin, const QVariant& end) const;
	/**
	 * @brief 结束动画开始
	*/
	virtual void outAnimationStart();
private:

	int m_duration = 1000;
};
