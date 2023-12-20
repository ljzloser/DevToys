#pragma once

#include <QWidget>
#include <qevent.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qcolor.h>
#include <QPropertyAnimation>
#include <qpushbutton.h>

/**
 * @brief 滑动按钮
*/
class SlideButton : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(QPointF position READ position WRITE setPosition)
public:
	SlideButton(QWidget* parent = nullptr, QVector<QString> names = { "关闭","打开" });
	~SlideButton();
	/**
	 * @brief 点击
	 * @return
	*/
	bool click();
	bool isChecked() const;
	/**
	 * @brief 设置默认选中
	 * @param checked
	*/
	void setDefaultChecked(bool checked) { this->m_isChecked = checked; this->NameLabel->setText(m_names[checked]); }

	void setborderColor(QColor color) { this->m_borderColor = color; }
	void setborderColorHover(QColor color) { this->m_borderColorHover = color; }
	void setbackgroundColor(QColor color) { this->m_backgroundColor = color; }
	void setbackgroundColorChecked(QColor color) { this->m_backgroundColorChecked = color; }
	void setbuttonColor(QColor color) { this->m_buttonColor = color; }
	void setbuttonColorHover(QColor color) { this->m_buttonColorHover = color; }
	/**
	 * @brief 设置名称
	 * @param names 第一个为关闭，第二个为开
	*/
	void setName(QVector<QString> names) { this->m_names = names; this->NameLabel->setText(m_names[this->isChecked()]); }

	QColor borderColor() const { return this->m_borderColor; }
	QColor borderColorHover() const { return this->m_borderColorHover; }
	QColor backgroundColor() const { return this->m_backgroundColor; }
	QColor backgroundColorChecked() const { return this->m_backgroundColorChecked; }
	QColor buttonColor() const { return this->m_buttonColor; }
	QColor buttonColorHover() const { return this->m_buttonColorHover; }
	QStringList name() const { return this->m_names; }
signals:
	/**
	 * @brief 点击信号
	*/
	void clicked(bool isChecked);

private slots:
	/**
	 * @brief 更新位置
	*/
	void setPosition(QPointF newValue);
	QPointF position() { return m_position; }

private:
	void mouseReleaseEvent(QMouseEvent* event);
	void leaveEvent(QEvent* event);
	void enterEvent(QEnterEvent* event);
	void paintEvent(QPaintEvent* event);
	void resizeEvent(QResizeEvent* event);

	QColor m_borderColor;
	QColor m_borderColorHover;
	QColor m_backgroundColor;
	QColor m_backgroundColorChecked;
	QColor m_buttonColor;
	QColor m_buttonColorHover;

	bool m_isChecked = false;
	bool m_isHover = false;
	QLabel* NameLabel = new QLabel();
	QWidget* button = new QWidget();
	QPointF m_position;
	QPropertyAnimation* animation = new QPropertyAnimation(this, "position");
	QPointF left;
	QPointF right;
	QVector<QString> m_names = { "关闭","打开" };
};
