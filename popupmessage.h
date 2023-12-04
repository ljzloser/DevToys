#pragma once

#include <QLabel>
#include <QTimer>
#include <qcursor.h>
#include <qpoint.h>
/**
 * @brief 弹出式消息框
*/
class PopupMessage  : public QLabel
{
	Q_OBJECT

public:
	PopupMessage(QWidget *parent);
	~PopupMessage();
private:
	void showEvent(QShowEvent* event);
};
