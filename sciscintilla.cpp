#include "sciscintilla.h"

SciScintilla::SciScintilla(QWidget *parent)
	: QsciScintilla(parent)
{
	this->setIndentationsUseTabs(false); // 不使用制表符
	this->setIndentationGuides(true); // 显示缩进线
	this->setAutoIndent(true); // 自动缩进
	this->setMarginType(0, QsciScintilla::MarginType::NumberMargin);// 设置行号
	this->setMarginWidth(0, 20); // 设置行号宽度
	this->setFolding(QsciScintilla::FoldStyle::PlainFoldStyle, 1); // 折叠
	this->setAutoCompletionSource(QsciScintilla::AutoCompletionSource::AcsAll); // 设置自动完成
	this->setAutoCompletionThreshold(1); // 设置自动完成阈值
	this->setWrapMode(QsciScintilla::WrapMode::WrapWord); // 设置自动换行
	connect(this, &QsciScintilla::linesChanged, [=]() {
		this->setMarginWidth(0, QString::number(this->lines()) + "0");
		});
}

SciScintilla::~SciScintilla()
{}
