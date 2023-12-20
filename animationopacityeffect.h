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
	 * @brief ���ö����Ŀ�ʼ�ͽ���
	 * @param animation ���Զ���ָ��
	 * @param begin ��ʼ
	 * @param end ����
	*/
	virtual void setAnimation(QPropertyAnimation* animation, const QVariant& begin, const QVariant& end) const;
	/**
	 * @brief ����������ʼ
	*/
	virtual void outAnimationStart();
private:

	int m_duration = 1000;
};
