#pragma once

#include <QLabel>
#include <QTimer>
#include <qcursor.h>
#include <qpoint.h>
#include <QPropertyAnimation>
#include "animationopacityeffect.h"
/**
 * @brief 弹出式消息框
*/
class PopupMessage  : public QLabel
{
	Q_OBJECT

public:
	PopupMessage(QWidget *parent);
	~PopupMessage();
	void setShowTime(int time) { this->m_showTime = time; };
private:
	void showEvent(QShowEvent* event);
	AnimationOpacityEffect* opacityEffect = new AnimationOpacityEffect(this);
	int m_showTime = 2000;
};
