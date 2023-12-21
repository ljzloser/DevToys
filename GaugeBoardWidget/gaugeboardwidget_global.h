#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(GAUGEBOARDWIDGET_LIB)
#  define GAUGEBOARDWIDGET_EXPORT Q_DECL_EXPORT
# else
#  define GAUGEBOARDWIDGET_EXPORT Q_DECL_IMPORT
# endif
#else
# define GAUGEBOARDWIDGET_EXPORT
#endif
