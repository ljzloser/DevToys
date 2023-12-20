#include "timestamp.h"

Timestamp::Timestamp(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->loadConnect();
	// 获取当前时区的偏移量
	QTimeZone timeZone = QTimeZone::systemTimeZone();
	int offset = timeZone.offsetFromUtc(QDateTime::currentDateTime());
	ui.offsetSpinBox->setValue(offset / 60 / 60);
}
void Timestamp::changeMaxDay()
{
	int year = ui.yearSpinBox->value();
	int month = ui.MonthSpinBox->value();
	QDate date = QDate(year, month, 1);
	int maxDay = date.daysInMonth();
	ui.daySpinBox->setMaximum(maxDay);
}
Timestamp::~Timestamp()
{}

void Timestamp::setDateTime(bool isCurrent)
{
	// 判断发送信号的对象
	QSpinBox* spinBox1 = (QSpinBox*)sender();
	for (QSpinBox* spinBox : this->findChildren<QSpinBox*>())
	{
		disconnect(spinBox, &QSpinBox::valueChanged, this, &Timestamp::setDateTime);
	}
	// 世界标准时间
	QDateTime utc;
	if (isCurrent)
	{
		utc = QDateTime::currentDateTime().toUTC();
	}
	else
	{
		utc = QDateTime::fromSecsSinceEpoch((qint64)ui.timedoubleSpinBox->value()).toUTC();
	}
	if (spinBox1)
	{
		utc = QDateTime(QDate(ui.yearSpinBox->value(), ui.MonthSpinBox->value(), ui.daySpinBox->value()),
			QTime(ui.HourSpinBox->value(), ui.MinutesSpinBox->value(), ui.secondsSpinBox->value()));
		utc = utc.addSecs(-1 * ui.offsetSpinBox->value() * 60 * 60);
	}
	ui.UtcTimestampLabel->setText(QString::number(utc.toMSecsSinceEpoch()));
	ui.WordDateTimeLable->setText(utc.toString("yyyy-MM-dd hh:mm:ss"));

	int offset = ui.offsetSpinBox->value();
	// 根据偏移量计算当前时间
	QDateTime dateTime = utc.addSecs(offset * 60 * 60);
	ui.DateTimeLabel->setText(dateTime.toString("yyyy-MM-dd hh:mm:ss"));
	if (isCurrent)
	{
		ui.timedoubleSpinBox->setValue(dateTime.toSecsSinceEpoch());
	}

	ui.yearSpinBox->setValue(dateTime.date().year());
	ui.MonthSpinBox->setValue(dateTime.date().month());
	ui.daySpinBox->setValue(dateTime.date().day());
	ui.HourSpinBox->setValue(dateTime.time().hour());
	ui.MinutesSpinBox->setValue(dateTime.time().minute());
	ui.secondsSpinBox->setValue(dateTime.time().second());
	for (QSpinBox* spinBox : this->findChildren<QSpinBox*>())
	{
		connect(spinBox, &QSpinBox::valueChanged, this, &Timestamp::setDateTime);
	}
}

void Timestamp::loadConnect()
{
	connect(ui.yearSpinBox, &QSpinBox::valueChanged, this, &Timestamp::changeMaxDay);
	connect(ui.MonthSpinBox, &QSpinBox::valueChanged, this, &Timestamp::changeMaxDay);
	for (QSpinBox* spinBox : this->findChildren<QSpinBox*>())
	{
		connect(spinBox, &QSpinBox::valueChanged, this, &Timestamp::setDateTime);
	}

	connect(ui.CurrDatePushButton, &QPushButton::clicked, [=]() {this->setDateTime(true); });
	connect(ui.timedoubleSpinBox, &QDoubleSpinBox::valueChanged, [=]() {this->setDateTime(false); });
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(QString::number((qint64)ui.timedoubleSpinBox->value())); });
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.timedoubleSpinBox->setValue(Tools::getClipboard().toDouble()); });
}

void Timestamp::showEvent(QShowEvent* event)
{
	this->setDateTime();
}