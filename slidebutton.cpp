#include "slidebutton.h"

SlideButton::SlideButton(QWidget* parent, QVector<QString> names)
	: QWidget(parent), m_names(names)
{
	this->setDefaultChecked(false);
	this->setborderColor(QColor(0, 0, 0));
	this->setborderColorHover(QColor(127, 127, 127));
	this->setbackgroundColorChecked(QColor("#07a4bb"));
	this->setbackgroundColor(QColor(255, 255, 255));
	this->setbuttonColor(QColor(0, 0, 0));
	this->setbuttonColorHover(QColor(0, 0, 0));

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	button->setStyleSheet("background: transparent;");
	this->NameLabel->setFixedWidth(30);

	layout->addWidget(this->NameLabel);
	layout->addWidget(this->button);

	this->setLayout(layout);

	this->animation->setDuration(100);
	connect(this->animation, &QPropertyAnimation::finished, [=]()
		{
			if (this->isChecked())
			{
				this->animation->setStartValue(right);
				this->animation->setEndValue(left);
			}
			else
			{
				this->animation->setStartValue(left);
				this->animation->setEndValue(right);
			}
			this->m_position = this->animation->startValue().toPointF();
		});
}

SlideButton::~SlideButton()
{}

void SlideButton::setPosition(QPointF newValue)
{
	this->m_position = newValue;
	update();
}

bool SlideButton::click()
{
	this->animation->start();
	this->m_isChecked = !this->m_isChecked;
	this->NameLabel->setText(m_names[this->isChecked()]);
	emit clicked(this->isChecked());
	return this->isChecked();
}

bool SlideButton::isChecked() const
{
	return this->m_isChecked;
}

void SlideButton::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		this->click();
	}
}

void SlideButton::enterEvent(QEnterEvent* event)
{
	this->m_isHover = true;
}

void SlideButton::leaveEvent(QEvent* event)
{
	this->m_isHover = false;
}

void SlideButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	QColor borderColor = this->m_isHover ? this->borderColorHover() : this->borderColor();
	QColor backgroundColor = this->m_isChecked ? this->backgroundColorChecked() : this->backgroundColor();
	QColor buttonColor = this->m_isHover ? this->buttonColorHover() : this->buttonColor();

	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(borderColor);

	painter.setBrush(this->backgroundColor());
	QRectF rect = this->button->geometry();
	painter.drawRoundedRect(rect, 10, 10);

	double radius = qMin((this->button->width() - 5) / 2.0 - 5, (this->button->height() - 5) / 2.0);
	QPointF point = this->button->mapToParent(this->m_position);

	if (this->isChecked())
	{
		rect.setRight(point.x() + radius + 2.5);
		painter.setPen(Qt::NoPen);
		painter.setBrush(backgroundColor);
		painter.drawRoundedRect(rect, 10, 10);
	}

	painter.setBrush(buttonColor);
	painter.drawEllipse(point, radius, radius);
	update();
}

void SlideButton::resizeEvent(QResizeEvent* event)
{
	double radius = qMin((this->button->width() - 5) / 2.0 - 5, (this->button->height() - 5) / 2.0);
	this->left = QPointF(radius + 5, this->button->height() / 2.0);
	this->right = QPointF(this->button->width() - radius - 2.5, this->button->height() / 2.0);
	if (this->isChecked())
	{
		this->animation->setStartValue(right);
		this->animation->setEndValue(left);
	}
	else
	{
		this->animation->setStartValue(left);
		this->animation->setEndValue(right);
	}
	this->m_position = this->animation->startValue().toPointF();
}