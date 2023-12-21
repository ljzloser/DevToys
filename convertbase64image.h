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
	/**
	 * @brief 当文本编辑器内容改变时调用此函数
	 *
	 * 此函数处理从Base64编码的图片字符串的解析，并展示在界面上。如果文本编辑器为空或者
	 * 图片数据不符合预期的格式，则会清空当前的图片并显示错误信息。解析成功时，图片会被转换
	 * 为QPixmap对象，并根据标签的尺寸进行缩放后展示。
	 */
	void onPlainTextEditChanged();
	/**
	 * @brief 加载图片文件并转换为Base64编码的字符串
	 *
	 * 函数接收一个文件名参数，使用QImageReader来读取图片文件。如果文件名非空且文件格式有效，
	 * 则将图片转换为Base64编码的字符串，并显示在文本编辑器中。函数还会处理图片格式的特殊情况，
	 * 如将jpg转换为jpeg，以适应数据URL的格式要求。如果图片加载失败，则会弹出错误信息。
	 *
	 * @param fileName 要加载的图片文件的路径。
	 */
	void onLoadImage(QString fileName);
	/**
	 * @brief 保存当前加载的图片为文件
	 *
	 * 此函数将当前在界面上显示的图片保存到文件。如果当前没有图片或图片类型为空，则会显示错误信息。
	 * 保存对话框允许用户选择文件保存位置和文件名。如果保存操作成功，会通知用户保存成功；否则，会警告用户保存失败。
	 */
	void onSaveImage();

private:
	Ui::ConvertBase64ImageClass ui;
	void loadConnect();
	void resizeEvent(QResizeEvent* event);
	// 拖入事件
	void dragEnterEvent(QDragEnterEvent* event) override;
	// 拖入松手后事件
	void dropEvent(QDropEvent* event) override;
	void dragMoveEvent(QDragMoveEvent* event) override;
	QString imageType;
	QPixmap pixmap;

	// 检查位置是否在希望允许拖放的区域内
	bool isWithinDropArea() const
	{
		QPoint pos = ui.frame_3->mapFromGlobal(QCursor::pos());
		QRect dropArea = ui.frame_3->rect();
		return dropArea.contains(pos);
	}
};
