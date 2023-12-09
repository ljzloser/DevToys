#include "passwordgeneration.h"

PasswordGeneration::PasswordGeneration(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.lowerButton->setDefaultChecked(true);
	ui.numberButton->setDefaultChecked(true);
	this->loadConnect();
}

PasswordGeneration::~PasswordGeneration()
{}

void PasswordGeneration::loadConnect()
{
	for (SlideButton* button : this->findChildren<SlideButton*>())
	{
		connect(button, &SlideButton::clicked, this, &PasswordGeneration::onSetButtonClicked);
	}
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.exportsTextEdit->toPlainText()); });
	connect(ui.clearButton, &QPushButton::clicked, ui.exportsTextEdit, &QPlainTextEdit::clear);
	connect(ui.generateButton, &QPushButton::clicked, this, &PasswordGeneration::onGenerateButtonClicked);
}

void PasswordGeneration::onSetButtonClicked()
{
	bool flag = false;
	for (SlideButton* button : this->findChildren<SlideButton*>())
	{
		flag |= button->isChecked();
	}
	if (!flag)
	{
		SlideButton* button = static_cast<SlideButton*>(sender());
		button->click();
	}
}

void PasswordGeneration::onGenerateButtonClicked()
{
	QList<int> type;
	int totalLength;
	try
	{
		totalLength = ui.lenSpinBox->value() * ui.countspinBox->value();
	}
	// 超出int范围
	catch (const std::exception&)
	{
		Tools::showPopupMessage("密码长度超出int范围");
	}

	const int numThreads = QThread::idealThreadCount();
	const int numPerThread = totalLength / numThreads;
	int count = ui.countspinBox->value();
	if (ui.lowerButton->isChecked()) type.append(0);
	if (ui.upperButton->isChecked()) type.append(1);
	if (ui.numberButton->isChecked()) type.append(2);
	if (ui.notationButton->isChecked()) type.append(3);
	bool flag = totalLength > 100000;
	QProgressDialog progressDialog(this);
	if (flag)
	{

		progressDialog.setLabelText("正在生成密码...");
		progressDialog.setRange(0, totalLength);
		progressDialog.setCancelButton(nullptr);
		progressDialog.setWindowModality(Qt::WindowModal);
		progressDialog.setWindowFlags(progressDialog.windowFlags()  & ~Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
	}

	QAtomicInt atomicInt(0);
	QString text;
	// 预分配
	text.fill(' ', totalLength);
	QMutex mutex;
	QThreadPool pool;
	for (int i = 0; i < numThreads; ++i)
	{
		int offset = i * numPerThread;
		int length = (i == numThreads - 1) ? totalLength - offset : numPerThread;
		if (length == 0) continue;
		RandomStringTask* task = new RandomStringTask(length, &text, &mutex, offset, &atomicInt, type);
		pool.start(task);
		if (flag)
		{
			connect(task, &RandomStringTask::progress, &progressDialog, &QProgressDialog::setValue);
		}
	}
	if (flag) progressDialog.exec();
	pool.waitForDone();
	ui.exportsTextEdit->setUpdatesEnabled(false);
	ui.exportsTextEdit->blockSignals(true);
	for (int i = 0; i < count; i++)
	{
		ui.exportsTextEdit->appendPlainText(text.mid(static_cast<qsizetype>(i) * ui.lenSpinBox->value(), ui.lenSpinBox->value()));
	}
	ui.exportsTextEdit->blockSignals(false);
	ui.exportsTextEdit->setUpdatesEnabled(true);
	ui.exportsTextEdit->update();
}
