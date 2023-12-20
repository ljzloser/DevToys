#pragma once

#include "tools.h"
#include "ui_convertbase64image.h"
#include <qcursor.h>
#include <QDesktopServices>
#include <qevent.h>
#include <qimagereader.h>
#include <qmimedata.h>
#include <QTemporaryFile>
#include <QWidget>
class ConvertBase64Image : public QWidget
{
	Q_OBJECT

public:
	ConvertBase64Image(QWidget* parent = nullptr);
	~ConvertBase64Image();

public slots:
	void onPlainTextEditChanged();
	void onLoadImage(QString fileName);
	void onSaveImage();

private:
	Ui::ConvertBase64ImageClass ui;
	void loadConnect();
	void resizeEvent(QResizeEvent* event);
	// 拖入事件
	void dragEnterEvent(QDragEnterEvent* event);
	// 拖入松手后事件
	void dropEvent(QDropEvent* event);
	void dragMoveEvent(QDragMoveEvent* event);
	QString imageType;
	QPixmap pixmap;
	// 检查位置是否在您希望允许拖放的区域内

	bool isWithinDropArea() const
	{
		QPoint pos = ui.frame_3->mapFromGlobal(QCursor::pos());
		QRect dropArea = ui.frame_3->rect();
		return dropArea.contains(pos);
	}
};
