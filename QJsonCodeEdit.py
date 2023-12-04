import json
import sys

from PyQt6.Qsci import *
from PyQt6.QtCore import *
from PyQt6.QtGui import *
from PyQt6.QtWidgets import *


class FindReplace(QDialog):
    find = pyqtSignal(str, int)

    def __init__(self, parent=None):
        super().__init__(parent)
        # 无标题栏，无边框,无任务栏图标
        self.setWindowFlags(Qt.WindowType.FramelessWindowHint | Qt.WindowType.Tool)
        self.setWindowTitle("查找")
        layout = QHBoxLayout()
        self.lineEdit = QLineEdit()
        self.lineEdit.setPlaceholderText("查找")
        # self.findCombobox = QComboBox()
        # self.findCombobox.addItems(["起始位置","当前位置"])
        layout.addWidget(self.lineEdit)
        # layout.addWidget(self.findCombobox)
        self.setLayout(layout)
        self.lineEdit.returnPressed.connect(lambda: self.find.emit(self.lineEdit.text(), 1))
        self.close_button = QPushButton()
        self.close_button.setFixedSize(25, 25)
        self.close_button.clicked.connect(self.hide)
        self.close_button.setText("X")
        self.close_button.setFocusPolicy(Qt.FocusPolicy.NoFocus)
        # layout.addWidget(self.findbutton)
        layout.addWidget(self.close_button)
        layout.setContentsMargins(3, 3, 3, 3)


class QJsonCodeEdit(QsciScintilla):
    def __init__(self, parent=None):
        super().__init__(parent)
        lexer = QsciLexerJSON()
        api = QsciAPIs(lexer)
        api.prepare()
        self.setLexer(lexer)
        self.setIndentationsUseTabs(False)
        self.setTabWidth(4)
        self.setIndentationGuides(True)
        self.setAutoIndent(True)
        self.setMarginType(0, QsciScintilla.MarginType.NumberMargin)
        self.setMarginWidth(0, 20)
        self.setFolding(QsciScintilla.FoldStyle.PlainFoldStyle, 1)
        self.setAutoCompletionSource(QsciScintilla.AutoCompletionSource.AcsAll)
        self.setAutoCompletionThreshold(1)
        self.setBraceMatching(QsciScintilla.BraceMatch.StrictBraceMatch)  # 括号匹配显示
        self.setWrapMode(QsciScintilla.WrapMode.WrapWord)
        self.setContextMenuPolicy(Qt.ContextMenuPolicy.CustomContextMenu)
        self.customContextMenuRequested.connect(self.showMenu)
        self.dialog = FindReplace(self)
        self.dialog.find.connect(self.findString)
        # 行数改变后，计算行号所需的宽度，并设置宽度
        self.linesChanged.connect(self.setLineNumbersWidth)
        if parent:
            parent.installEventFilter(self)

    @pyqtSlot(str, int)
    def findString(self, text: str, index: int):
        if not self.findNext():
            self.findFirst(text, 0, 0, 0, index, 1, 1, 0, 0)

    def setLineNumbersWidth(self):
        self.setMarginWidth(0, str(self.lines()) + '0')

    def keyPressEvent(self, e) -> None:
        keyDict = {Qt.Key.Key_BraceLeft: '}', Qt.Key.Key_BracketLeft: "]", Qt.Key.Key_ParenLeft: ')',
                   Qt.Key.Key_Apostrophe: "'", Qt.Key.Key_QuoteDbl: '"'}
        # 如果按下的是左括号
        if e.key() in keyDict:
            # 在当前光标后添加括号
            self.insert(keyDict[e.key()])
        if e.modifiers() == Qt.KeyboardModifier.ControlModifier and e.key() == Qt.Key.Key_G:
            self.codeFormat()
        if e.modifiers() == Qt.KeyboardModifier.ControlModifier and e.key() == Qt.Key.Key_F:
            self.showFindReplaceDialog()
        return super().keyPressEvent(e)

    def codeFormat(self):
        text = self.text()
        try:
            text = json.dumps(json.loads(text), indent=4, ensure_ascii=False)
            self.selectAll()
            self.replaceSelectedText(text)
        except Exception as e:
            QMessageBox.warning(self, "警告", e.__str__())

    @pyqtSlot()
    def showFindReplaceDialog(self):
        text = self.selectedText()
        self.dialog.lineEdit.setText(text)
        self.dialog.show()
        self.dialog.activateWindow()
        self.dialog.lineEdit.setFocus()
        self.findDialogMove()

    @pyqtSlot()
    def findDialogMove(self):
        rect = self.dialog.rect()
        # 如果比当前窗口大就设置为当前窗口大小
        if rect.width() > self.width():
            rect.setWidth(self.width())
            self.dialog.hide()
            return
        if rect.height() > self.height():
            rect.setHeight(self.height())
            self.dialog.hide()
            return
        # 将dialog的右上角设置为当前窗口的右上角
        rect.moveTopRight(self.mapToGlobal(self.rect().topRight()))
        self.dialog.move(rect.topLeft())

    def moveEvent(self, a0: QMoveEvent) -> None:
        self.findDialogMove()
        return super().moveEvent(a0)

    def resizeEvent(self, e: QResizeEvent) -> None:
        self.findDialogMove()
        return super().resizeEvent(e)

    def eventFilter(self, a0: QObject, a1: QEvent) -> bool:
        if a1.type() == QEvent.Type.Move:
            self.findDialogMove()
        return super().eventFilter(a0, a1)

    def showEvent(self, e):
        parent = self.parent()
        while parent.parent():
            parent = parent.parent()
        if parent:
            parent.installEventFilter(self)

    @pyqtSlot()
    def showMenu(self):
        menu = self.createStandardContextMenu()
        formatAction = menu.addAction(self.tr("Format"), self.codeFormat)
        formatAction.setShortcut(QKeySequence("Ctrl+G"))
        findAction = menu.addAction(self.tr("Find"), self.showFindReplaceDialog)
        findAction.setShortcut(QKeySequence("Ctrl+F"))
        chinese = ["撤销(U)", "重做(R)", "", "剪切(X)", "复制(C)", "粘贴(P)", "删除", "", "全选(A)", "格式化(G)",
                   "查找(F)"]
        # 遍历所有的action
        for i, action in enumerate(menu.actions()):
            action.setText(chinese[i])

        menu.exec(QCursor().pos())

    def tr(self, msg):
        return QCoreApplication.translate("@default", msg)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    mainWindow = QJsonCodeEdit()
    mainWindow.setText('''{
    "name": "张三",
    "age": 18,
    "data": {
        "name": "李四"
        }
}''')
    mainWindow.show()
    # 加载翻译文件
    translate = QTranslator()
    flag = translate.load("qt_zh_CN.qm")
    app.installTranslator(translate)
    sys.exit(app.exec())
