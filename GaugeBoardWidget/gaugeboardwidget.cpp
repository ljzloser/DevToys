#include "gaugeboardwidget.h"

GaugeBoardWidget::GaugeBoardWidget(double minValue, double maxValue, int precision, QWidget* parent)
	:QWidget(parent)
{
	this->setRange(minValue, maxValue, precision);
	this->init();
}
GaugeBoardWidget::GaugeBoardWidget(int minValue, int maxValue, QWidget* parent)
	:QWidget(parent)
{
	this->setRange(minValue, maxValue);
	this->init();
}
GaugeBoardWidget::~GaugeBoardWidget()
{
}
void GaugeBoardWidget::init()
{
	this->drawValue = minValue;
	this->colorMap = QMap<ColorKey, QColor>({
		{ColorKey::Background,QColor(Qt::GlobalColor::transparent)},
		{ColorKey::MaxCir,QColor(Qt::GlobalColor::darkCyan)},
		{ColorKey::MidCir,QColor(Qt::GlobalColor::lightGray)},
		{ColorKey::Range,QColor(Qt::red)},
		{ColorKey::MinCir,QColor(Qt::GlobalColor::black)},
		{ColorKey::Scale,QColor(Qt::GlobalColor::black)},
		{ColorKey::Number,QColor(Qt::GlobalColor::magenta)},
		{ColorKey::CenterCir,QColor(Qt::GlobalColor::darkCyan)},
		{ColorKey::Ptr,QColor(Qt::blue)},
		{ColorKey::Value,QColor(Qt::GlobalColor::white)},
		{ColorKey::Name,QColor(Qt::GlobalColor::lightGray)}
		});
	connect(this->timer, &QTimer::timeout, this, &GaugeBoardWidget::timeOutSlot);
	this->timer->start(this->timeChange);
	//    this->setWindowFlags(Qt::FramelessWindowHint);
	//    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

double GaugeBoardWidget::valueToAngle()
{
	double oneAngle = (maxValue - minValue) / 270;
	double result = -(this->drawValue - maxValue) / oneAngle - 45;
	if (result < -45)
	{
		result = -45;
	}
	else if (result > 225)
	{
		result = 225;
	}
	return result;
}

double GaugeBoardWidget::angleToValue(int angle)
{
	double oneAngle = (maxValue - minValue) / 270;
	return maxValue - (angle + 45) * oneAngle;
}

void GaugeBoardWidget::timeOutSlot()
{
	double oneAngle = (maxValue - minValue) / 270 * 5;
	if (this->drawValue < this->value)
	{
		this->drawValue = qMin(this->drawValue + oneAngle, value);
		update();
	}
	else
	{
		this->drawValue = qMax(this->drawValue - oneAngle, value);
		update();
	}
}
void GaugeBoardWidget::setRange(double minValue, double maxValue, int precision)
{
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->precision = precision;
}
void GaugeBoardWidget::setRange(int minValue, int maxValue)
{
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->precision = 0;
}

void GaugeBoardWidget::setColor(ColorKey key, QColor color)
{
	this->colorMap[key] = color;
}

void GaugeBoardWidget::setColor(QMap<ColorKey, QColor>& colorMap)
{
	this->colorMap = colorMap;
}
double GaugeBoardWidget::getMinValue()
{
	return this->minValue;
}
double GaugeBoardWidget::getMaxValue()
{
	return this->maxValue;
}
int GaugeBoardWidget::getPrecision()
{
	return this->precision;
}
void GaugeBoardWidget::setValue(int value)
{
	this->setValue((double)value);
}
void GaugeBoardWidget::setValue(double value)
{
	this->value = value;
	if (!this->isOpenAnimation())
	{
		this->drawValue = value;
	}
	emit this->valueChange(this->value);
}

void GaugeBoardWidget::setUnit(QString unit)
{
	this->unit = unit;
}

void GaugeBoardWidget::setName(QString Name)
{
	this->name = Name;
}

void GaugeBoardWidget::setOpenAnimation(bool isOpen)
{
	if (this->m_isOpenAnimation == isOpen)
	{
		return;
	}

	this->m_isOpenAnimation = isOpen;
	if (isOpen)
	{
		this->timer->start(this->timeChange);
	}
	else
	{
		this->timer->stop();
		this->drawValue = this->value;
	}
}
double GaugeBoardWidget::getValue()
{
	return this->value;
}

const QMap<GaugeBoardWidget::ColorKey, QColor>& GaugeBoardWidget::getColor()
{
	return this->colorMap;
}

QString GaugeBoardWidget::getUnit()
{
	return this->unit;
}

QColor GaugeBoardWidget::getColor(ColorKey key)
{
	return this->colorMap[key];
}

void GaugeBoardWidget::paintEvent(QPaintEvent* e)
{
	// 获取需要绘制的区域
	QRect rect = e->rect();

	// 创建 QPainter 对象，并指定绘图设备为当前窗口
	QPainter painter(this);
	painter.setPen(this->colorMap[ColorKey::Background]);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(this->colorMap[ColorKey::Background]);

	// 绘制背景
	painter.drawRect(rect);
	QRect boardRect = QRect(20, 20, this->width() - 40, this->height() - 40);

	int side = qMin(boardRect.width(), boardRect.height()); // 获取较小尺寸
	// 画仪表盘外圆
	painter.setPen(this->colorMap[ColorKey::MaxCir]);
	painter.setBrush(this->colorMap[ColorKey::MaxCir]);
	painter.drawEllipse(boardRect.center(), side / 2, side / 2);

	// 画仪表盘中圆

	//    painter.setPen(this->colorMap[ColorKey::MidCir]);
	//    painter.setBrush(this->colorMap[ColorKey::MidCir]);
	//    painter.drawEllipse(boardRect.center(),int(side / 2 / 20 * 19) ,int(side / 2 / 20 * 19));

	QRectF outRect(0, 0, int(side / 20 * 19), int(side / 20 * 19));//矩形长宽为窗口的长宽
	outRect.moveCenter(boardRect.center());

	painter.setPen(Qt::NoPen);
	painter.setBrush(QBrush(QColor(this->colorMap[ColorKey::MidCir])));
	painter.drawEllipse(outRect);

	QConicalGradient gradient;
	// 定义渐变色
	gradient.setColorAt(0, this->colorMap[ColorKey::MinCir]);
	gradient.setColorAt(0.5, this->colorMap[ColorKey::MidCir]);
	gradient.setColorAt(1, this->colorMap[ColorKey::Range]);

	painter.setBrush(gradient);
	painter.drawPie(outRect, (225) * 16, -(225 - valueToAngle()) * 16);

	// 画仪表盘内圆

	painter.setPen(this->colorMap[ColorKey::MinCir]);
	painter.setBrush(this->colorMap[ColorKey::MinCir]);
	painter.drawEllipse(boardRect.center(), int(side / 2 / 20 * 15), int(side / 2 / 20 * 15));

	// 画仪表盘刻度
	QPen ScalePen = QPen(this->colorMap[ColorKey::Scale]);
	ScalePen.setWidth(2);
	painter.setPen(ScalePen);
	QList<QPointF> points;
	for (int i = -45; i <= 225; i += 5) {
		qreal rad = qDegreesToRadians(double(i));
		int k = i % 45 == 0 ? 15 : 17;
		QPointF outerPoint((boardRect.center().x() + (side / 2 / 20 * 19) * qCos(rad)),
			(boardRect.center().y() - (side / 2 / 20 * 19) * qSin(rad)));
		QPointF innerPoint((boardRect.center().x() + (side / 2 / 20 * k) * qCos(rad)),
			(boardRect.center().y() - (side / 2 / 20 * k) * qSin(rad)));
		painter.drawLine(outerPoint, innerPoint);
		if (k == 15)
		{
			points.append(innerPoint);
		}
		//painter.drawText(outerPoint, QString::number(i));
	}

	//    QPen pen1(QColor(this->colorMap[ColorKey::CenterCir]));
	//    pen1.setWidth(2);
	//    painter.setPen(pen1);
	//    for (int i = 0; i < points.size(); i++ )
	//    {
	//        int begin = i;
	//        int end = i + 1 == points.size() ? 0 : i + 1;
	//        painter.drawLine(points[begin],points[end]);
	////        painter.drawLine(points[begin],boardRect.center());
	//    }

	// 绘制文字
	QFont font = QFont("Arial", side / 30);
	painter.setFont(font);
	painter.setPen(this->colorMap[ColorKey::Number]);
	for (int i = -45; i <= 225; i += 45) {
		qreal rad = qDegreesToRadians(double(i));
		QPointF outerPoint((boardRect.center().x() + (side / 2 / 20 * 12) * qCos(rad)),
			(boardRect.center().y() - (side / 2 / 20 * 12) * qSin(rad)));
		QString text = QString("%1%2").arg(QString::number(this->angleToValue(i), 'f', this->precision)).arg("");
		// 计算文字的大小
		QFontMetrics fm(font);
		QRectF textRect = fm.boundingRect(text);
		textRect.moveCenter(outerPoint);
		painter.drawText(textRect, Qt::AlignCenter, text); // 绘制文字
	}
	// 画指针
	QPen pen(Qt::red);
	pen.setWidth(side / 60);
	painter.setPen(pen);
	painter.setBrush(Qt::red);
	qreal rad = qDegreesToRadians(valueToAngle());
	QPoint center = boardRect.center();
	int radius = side / 2 / 20 * 10;
	QPoint hand(static_cast<int>(width() / 2 + radius * qCos(rad)),
		static_cast<int>(height() / 2 - radius * qSin(rad)));
	painter.drawLine(center, hand);

	// 画仪表盘中心圆

	painter.setPen(this->colorMap[ColorKey::CenterCir]);
	painter.setBrush(this->colorMap[ColorKey::CenterCir]);
	painter.drawEllipse(boardRect.center(), int(side / 2 / 20 * 2), int(side / 2 / 20 * 2));

	// 画数值
	QString text = QString("%1%2").arg(QString::number(this->drawValue, 'f', this->precision)).arg(this->unit);
	painter.setPen(this->colorMap[ColorKey::Value]);
	font = QFont("Arial", side / 20);
	painter.setFont(font);
	QPoint p(boardRect.center().x(), boardRect.center().y() + (side / 2 / 20 * 9));
	// 计算文字的大小
	QFontMetrics fm(font);
	QRect textRect = fm.boundingRect(text);
	textRect.moveCenter(p);
	painter.drawText(textRect, Qt::AlignCenter, text); // 绘制文字

	//画名称
	painter.setPen(this->colorMap[ColorKey::Name]);
	p = QPoint(boardRect.center().x(), boardRect.center().y() + (side / 2 / 20 * 12));
	textRect = fm.boundingRect(this->name);
	textRect.moveCenter(p);
	painter.drawText(textRect, Qt::AlignCenter, this->name); // 绘制文字
}

void GaugeBoardWidget::contextMenuEvent(QContextMenuEvent* e)
{
	QMenu menu(this);
	QAction copyAction("复制");
	menu.addAction(&copyAction);
	connect(&copyAction, &QAction::triggered, [=]()
		{
			QClipboard* clipboard = QApplication::clipboard();
			QString text = QString("%1%2").arg(QString::number(this->drawValue, 'f', this->precision)).arg(this->unit);
			clipboard->setText(text);
		});
	menu.exec(e->globalPos());
}