#include "jsontoyaml.h"
#include <qjsonobject.h>
#include <qlabel.h>
#include <qmessagebox.h>

JsonToYaml::JsonToYaml(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setUi();
	this->loadConnect();
}

JsonToYaml::~JsonToYaml()
{}
void JsonToYaml::onIndentComboBoxCurrentIndexChanged(int index)
{
	ui.importWidget->setTabWidth(index * 2 + 2); // 设置制表符宽度
	ui.importWidget->setIndentationWidth(index * 2 + 2); // 设置缩进宽度
	ui.exportsWidget->setTabWidth(index * 2 + 2); // 设置制表符宽度
	ui.exportsWidget->setIndentationWidth(index * 2 + 2); // 设置缩进宽度
	QString text = ui.exportsWidget->text();
	if (ui.ConvertComboBox->currentIndex() == 0)
	{

		try
		{
			YAML::Node node = YAML::Load(text.toStdString());
			text = Tools::formatYaml(node, index * 2 + 2);
		}
		catch (const YAML::ParserException& e)
		{

		}
	}
	else
	{
		Json::Value root;
		Json::Reader reader;

		if (reader.parse(text.toStdString(), root))
		{
			text = Tools::formatJson(root, index * 2 + 2);
		}
	}
	ui.exportsWidget->setText(text);
}

void JsonToYaml::onImportChanged()
{
	QString text = ui.importWidget->text();
	// 判断当前选择的是JSON还是YAML
	if (ui.ConvertComboBox->currentIndex() == 0)
	{
		Json::Value root;
		Json::Reader reader;

		if (!reader.parse(text.toStdString(), root)) {
			// 解析失败，获取失败的原因
			std::string error = reader.getFormattedErrorMessages();
			// 将失败的原因显示到界面上
			ui.exportsWidget->setText(QString::fromStdString(error));
		}
		else
		{
			YAML::Node yaml;
			Tools::convertJsonToYaml(root, yaml);
			QString str = Tools::formatYaml(yaml, ui.indentComboBox->currentIndex() == 0 ? 4 : 2);
			ui.exportsWidget->setText(str);
		}
	}
	else
	{
		try
		{
			YAML::Node node = YAML::Load(text.toStdString());
			Json::Value root;
			Tools::convertYamlToJson(node, root);
			QString str = Tools::formatJson(root, ui.indentComboBox->currentIndex() == 0 ? 4 : 2);
			ui.exportsWidget->setText(str);
		}
		catch (const YAML::ParserException& e)
		{
			ui.exportsWidget->setText(e.what());
		}
	}
}
void JsonToYaml::onConvertComboBoxCurrentIndexChanged(int index)
{
	delete ui.importWidget->lexer();
	delete ui.exportsWidget->lexer();
	QsciLexer* imporLexer = nullptr;
	QsciLexer* exportLexer = nullptr;
	if (ui.ConvertComboBox->currentIndex() == 0)
	{
		imporLexer = new QsciLexerJSON();
		exportLexer = new QsciLexerYAML();
	}
	else
	{
		imporLexer = new QsciLexerYAML();
		exportLexer = new QsciLexerJSON();
	}
	ui.importWidget->setLexer(imporLexer);
	ui.exportsWidget->setLexer(exportLexer);

}
void JsonToYaml::setUi()
{
	onConvertComboBoxCurrentIndexChanged(ui.ConvertComboBox->currentIndex());
	onIndentComboBoxCurrentIndexChanged(ui.indentComboBox->currentIndex());
}

void JsonToYaml::loadConnect()
{
	connect(ui.importWidget, &SciScintilla::textChanged, this, &JsonToYaml::onImportChanged);
	connect(ui.ConvertComboBox, &QComboBox::currentIndexChanged, this, &JsonToYaml::onConvertComboBoxCurrentIndexChanged);
	connect(ui.indentComboBox, &QComboBox::currentIndexChanged, this, &JsonToYaml::onIndentComboBoxCurrentIndexChanged);
	connect(ui.ClearButton, &QPushButton::clicked, ui.importWidget, &SciScintilla::clear);
	connect(ui.ClearButton, &QPushButton::clicked, ui.exportsWidget, &SciScintilla::clear);
	connect(ui.copyButton, &QPushButton::clicked, [=]() {Tools::setClipboard(ui.exportsWidget->text()); });
	connect(ui.pasteButton, &QPushButton::clicked, [=]() {ui.importWidget->setText(Tools::getClipboard()); });
	connect(ui.OpenFileButton, &QPushButton::clicked, [=]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), ".");
			if (!fileName.isEmpty())
			{
				QFile file(fileName);
				if (file.open(QIODevice::ReadOnly))
				{
					ui.importWidget->setText(file.readAll());
				}
				else
				{
					QMessageBox::warning(this, tr("警告"), file.errorString());
				}
			}
		});
	connect(ui.expandButton, &QPushButton::clicked, [=]() 
		{
			ui.setGroupBox->setVisible(!ui.setGroupBox->isVisible());
		});
}