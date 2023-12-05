#include "tools.h"


bool Tools::convertYamlToJson(const YAML::Node& ynode, Json::Value& jnode)
{
	try
	{
		if (ynode.IsScalar()) //如果是标量
		{
			Json::Value value(ynode.Scalar());//将标量赋值给Value
			jnode.swapPayload(value);// 交换
			return true;
		}
		if (ynode.IsSequence()) // 如果是数组
		{
			// 遍历数组
			for (int i = 0; i < ynode.size(); i++)
			{
				Json::Value value;
				// 递归调用 每个元素
				if (convertYamlToJson(ynode[i], value))
				{
					// 把每个元素放入数组
					jnode.append(value);
				}
				else
				{
					return false;
				}
			}
		}
		else if (ynode.IsMap()) // 如果是对象
		{
			// 遍历对象
			for (auto it = ynode.begin(); it != ynode.end(); ++it)
			{
				Json::Value value;
				// 递归调用
				if (convertYamlToJson(it->second, value))
				{
					// 根据key赋值node;
					jnode[it->first.Scalar()] = value;
				}
				else
				{
					return false;
				}
			}
		}
	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}

bool Tools::convertJsonToYaml(const Json::Value& jnode, YAML::Node& ynode)

{
	try
	{
		// 如果是数组
		if (jnode.isArray())
		{
			// 遍历数组
			for (int i = 0; i < jnode.size(); i++)
			{
				YAML::Node node;
				// 递归调用 每个元素
				if (convertJsonToYaml(jnode[i], node))
				{
					// 把每个元素放入数组
					ynode.push_back(node);
				}
				else
				{
					return false;
				}
			}
		}
		else if (jnode.isObject()) // 如果是对象
		{
			// 遍历对象
			for (auto it = jnode.begin(); it != jnode.end(); ++it)
			{
				YAML::Node node;
				// 递归调用
				if (convertJsonToYaml(*it, node))
				{
					// 根据key赋值node;
					ynode[it.name()] = node;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			// 如果是基本类型，直接赋值
			ynode = jnode.asString();
		}
	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}

QString Tools::formatIconPath(QString name)
{
	return QString("%1%2.png").arg(ICONDIR).arg(name);
}

QString Tools::formatJson(const Json::Value& jnode, int indent)
{
	Json::StreamWriterBuilder writer;
	writer.settings_["indentation"] = std::string(indent, ' ');
	std::string output = Json::writeString(writer, jnode);
	return QString::fromStdString(output);
}

QString Tools::formatYaml(const YAML::Node& ynode, int indent)
{
	YAML::Emitter out;
	out.SetIndent(indent);
	out << ynode;
	std::string str = out.c_str();
	return QString::fromStdString(str);
}

bool Tools::formatXml(const QString& xmlString, int indent, QString& formattedString, QString& errorStr)
{
	QXmlStreamReader reader(xmlString);
	QXmlStreamWriter writer(&formattedString);

	// 设置自动格式化和缩进级别
	writer.setAutoFormatting(indent > 0);
	writer.setAutoFormattingIndent(indent);
	Tools::showPopupMessage(QString::number(indent));
	while (!reader.atEnd()) {
		reader.readNext();

		// 处理潜在的错误
		if (reader.hasError()) {
			// 处理错误，例如记录错误信息
			errorStr = QString("parsing error: %1 in line:%2 col: %2").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber());
			// 根据应用需求选择性地处理错误
			return false;
		}

		// 忽略空白字符，除非缩进大于0
		if (indent > 0 || !reader.isWhitespace()) {
			writer.writeCurrentToken(reader);
		}
	}
	return true;
}

void Tools::setClipboard(QString text)
{
	// 设置剪切板
	QApplication::clipboard()->setText(text);
	// 显示气泡
	Tools::showPopupMessage("已复制到剪切板");
}

QString Tools::getClipboard()
{
	return QApplication::clipboard()->text();
}

void Tools::setClipboardPixmap(QPixmap pixmap)
{
	// 设置剪切板
	QApplication::clipboard()->setPixmap(pixmap);
	// 显示气泡
	Tools::showPopupMessage("已复制到剪切板");
}

QPixmap Tools::getClipboardPixmap()
{
	return QApplication::clipboard()->pixmap();
}

void Tools::showPopupMessage(QString content)
{
	// 获取应用程序对象
	QApplication* app = qobject_cast<QApplication*>(QCoreApplication::instance());

	// 使用应用程序对象获取主窗口指针
	if (app) {
		QWidget* widget = qobject_cast<QWidget*>(app->activeWindow());
		if (widget) {
			// 在这里使用主窗口指针	
			PopupMessage* message = new PopupMessage(widget);
			// 计算文本需要的Rect
			QFontMetrics fm = message->fontMetrics();
			QRect rect1 = fm.boundingRect(content);


			message->setFixedSize(rect1.width() + 30, rect1.height() + 30);

			QPoint p = widget->mapFromParent(QCursor::pos());
			p.setY(p.y() - 30);
			QRect rect = message->geometry();
			rect.moveBottomRight(p);
			message->setGeometry(rect);
			message->setText(content);
			message->show();
		}
	}
}

QString Tools::convertBase(const QString& number, int currentBase, int targetBase, bool formatOutput) {
	// 将输入的字符串转换为十进制数字
	bool ok;


	QRegularExpression separator("\\s");
	QString strippedNumber = number;
	strippedNumber.replace(separator, "");

	int groupSize = 4;
	if (targetBase == 2) {
		groupSize = 4;
		strippedNumber = strippedNumber.rightJustified((strippedNumber.length() / groupSize + 1) * groupSize, '0');
	}
	else if (targetBase == 8 || targetBase == 16) {
		groupSize = 4;
	}
	else if (targetBase == 10) {
		QRegularExpression separator(",");
		strippedNumber.replace(separator, "");
		groupSize = 3;
	}

	int decimalNumber = strippedNumber.toInt(&ok, currentBase);



	if (!ok)
	{
		Tools::showPopupMessage("请输入有效数字");
		return "";
	}

	// 将十进制数字转换为目标进制的字符串
	QString result = QString::number(decimalNumber, targetBase);
	// 格式化输出
	if (formatOutput)
	{
		int groupSize = 3;  // 默认每组3个数字
		QString separator = ",";  // 默认使用逗号分隔

		if (targetBase == 2)
		{
			groupSize = 4;  // 二进制每组4个数字
			separator = " ";  // 二进制使用空格分隔
			result = result.rightJustified((result.length() / groupSize + 1) * groupSize, '0');
		}
		else if (targetBase == 8 || targetBase == 16)
		{
			groupSize = 4;  // 八进制和十六进制每组4个数字
			separator = " ";  // 八进制和十六进制使用空格分隔
		}

		// 在适当的位置插入分隔符
		for (int i = result.length() - groupSize; i > 0; i -= groupSize)
		{
			result.insert(i, separator);
		}
	}

	return result;
}

QString Tools::htmlEncode(const QString& text)
{
	return text.toHtmlEscaped();
}

QString Tools::htmlDecode(const QString& text)
{
	QTextDocument textDocument;
	textDocument.setHtml(text);
	return textDocument.toPlainText();
}

QString Tools::urlEncode(const QString& plainText)
{
	QUrl url;
	url.setQuery(plainText);
	return url.query(QUrl::FullyEncoded);
}

QString Tools::urlDecode(const QString& encodedText)
{
	return QUrl::fromPercentEncoding(encodedText.toUtf8());
}

QString Tools::Base64Encode(const QString& inputString)
{
	QByteArray raw = inputString.toUtf8();
	return raw.toBase64();
}

QString Tools::Base64Decode(const QString& base64String)
{
	QByteArray encoded = QByteArray::fromBase64(base64String.toUtf8());
	return QString::fromUtf8(encoded);
}

