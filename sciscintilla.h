#pragma once
#include "Qsci/qsciscintilla.h"
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexerjson.h>
#include <Qsci/qscilexeryaml.h>
#include <Qsci/qsciscintillabase.h>
#include <Qsci/qscilexermarkdown.h>

/**
 * @brief QScintilla 的子类,用于扩展,使其支持自定义语法和样式。
 目前这个类里面还缺少部分自动补全和全局查找的功能。
*/
class SciScintilla : public QsciScintilla
{
	//Q_OBJECT

public:
	SciScintilla(QWidget* parent = nullptr);
	~SciScintilla();
};
