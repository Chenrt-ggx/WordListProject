import os
import re
import time

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

import core
import lib


class InputOptionView(QWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        layout = QHBoxLayout()
        layout.addWidget(QLabel("输入内容："))
        self.filePathView = QLineEdit()
        self.filePathView.setReadOnly(True)
        self.fileDialogButton = QPushButton("打开")
        self.fileDialogButton.clicked.connect(self.chooseFile)
        layout.addWidget(self.filePathView)
        layout.addWidget(self.fileDialogButton)
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

    def chooseFile(self):
        filename, t = QFileDialog.getOpenFileName(self, "打开文件", '', 'Text (*.txt)')
        self.filePathView.setText(filename)
        self.parent().loadPreview(filename)


class OutputOptionView(QWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        layout = QHBoxLayout()
        layout.addWidget(QLabel("输出内容："))
        self.fileExportButton = QPushButton("导出")
        layout.addStretch()
        self.fileExportButton.clicked.connect(self.exportFile)
        layout.addWidget(self.fileExportButton)
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

    def exportFile(self):
        filename, t = QFileDialog.getSaveFileName(self, '导出结果', 'result', "Text (*.txt)")
        if filename:
            self.parent().exportFile(filename)


class IOView(QWidget):
    def __init__(self, *args, **kwargs):
        self._input: bytes = b''
        self._output: bytes = b''
        super().__init__(*args, **kwargs)
        layout = QVBoxLayout()
        self.inputOptionView = InputOptionView(parent=self)
        self.inputPreview = QTextEdit(self)
        self.inputPreview.setAcceptRichText(False)
        layout.addWidget(self.inputOptionView)
        layout.addWidget(self.inputPreview)
        self.outputOptionView = OutputOptionView(parent=self)
        self.outputPreview = QTextEdit()
        self.outputPreview.setAcceptRichText(False)
        self.outputPreview.setReadOnly(True)
        layout.addWidget(self.outputOptionView)
        layout.addWidget(self.outputPreview)
        self.setLayout(layout)

    @property
    def input(self):
        self.input = lib.to_ascii(self.inputPreview.toPlainText().encode())
        return self._input

    @input.setter
    def input(self, new):
        self._input = new
        self.inputPreview.setText(self._input.decode())

    @property
    def output(self):
        return self._output

    @output.setter
    def output(self, new):
        self._output = new
        self.outputPreview.setText(self._output.decode())

    def loadPreview(self, filename):
        if os.path.exists(filename):
            with open(filename, 'rb') as f:
                content = f.read()
            harmless_content = lib.to_ascii(content)
            self.input = harmless_content
        else:
            self.input = b''

    def exportFile(self, filename):
        with open(filename, 'wb') as f:
            f.write(self.output)


class CharSelector(QLineEdit):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setMaxLength(1)
        self.setMaximumWidth(20)

    def keyPressEvent(self, a0: QKeyEvent) -> None:
        if Qt.Key_A <= a0.key() <= Qt.Key_Z:
            self.setText(chr(a0.key() - Qt.Key_A + ord('a')))

    def focusOutEvent(self, e: QFocusEvent):
        if not re.match('[a-z]', self.text()):
            self.setText('a')
            message = QMessageBox(self)
            message.setText("不错的hack尝试，但是")
            message.setInformativeText("我防出去啦！")
            message.setIcon(QMessageBox.Critical)
            message.exec_()
        super(CharSelector, self).focusOutEvent(e)


class BeginEndWithView(QWidget):
    def __init__(self, text, *args, **kwargs):
        super().__init__(*args, **kwargs)
        layout = QHBoxLayout()
        self.checkbox = QCheckBox()
        self.checkbox.stateChanged.connect(self.onCheckboxStateChanged)
        self.char = CharSelector()
        self.char.setDisabled(True)
        self.char.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.checkbox)
        layout.addWidget(QLabel('以'))
        layout.addWidget(self.char)
        layout.addWidget(QLabel(text))
        layout.addStretch()  # left align
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

    def mouseReleaseEvent(self, a0: QMouseEvent) -> None:
        self.checkbox.toggle()

    def onCheckboxStateChanged(self, state):
        if state == Qt.Checked:
            self.char.setDisabled(False)
            self.char.setText('a')
            self.char.setFocus()
        else:
            self.char.setDisabled(True)
            self.char.setText('')
        self.parent().refreshAvailableButton()

    def getCChar(self) -> int:
        if self.checkbox.isChecked():
            return ord(self.char.text().encode())
        else:
            return 0


class AllowCycleView(QWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        layout = QHBoxLayout()
        self.checkbox = QCheckBox()
        self.checkbox.stateChanged.connect(self.parent().refreshAvailableButton)
        layout.addWidget(self.checkbox)
        layout.addWidget(QLabel('允许输入单词环'))
        layout.addStretch()
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

    def mouseReleaseEvent(self, a0: QMouseEvent) -> None:
        self.checkbox.toggle()


class OptionView(QWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setFixedWidth(200)
        layout = QVBoxLayout()
        self.beginWithView = BeginEndWithView("开头", parent=self)
        layout.addWidget(self.beginWithView)
        self.endWithView = BeginEndWithView("结尾", parent=self)
        layout.addWidget(self.endWithView)
        self.allowCycleView = AllowCycleView(parent=self)
        layout.addWidget(self.allowCycleView)

        layout.addWidget(QLabel())

        self.queryChainsAllButton = QPushButton('查询链数')
        layout.addWidget(self.queryChainsAllButton)
        self.queryChainWordButton = QPushButton('查询最长单词数链')
        layout.addWidget(self.queryChainWordButton)
        self.queryChainCharButton = QPushButton('查询最长字母数链')
        layout.addWidget(self.queryChainCharButton)
        self.queryChainWordUniqueButton = QPushButton('查询最长单词数链\n（开头不重复）')
        layout.addWidget(self.queryChainWordUniqueButton)
        layout.addStretch()
        self.profileLabel = QLabel("")
        self.profileLabel.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.profileLabel)
        self.setLayout(layout)

    def refreshAvailableButton(self):
        if self.beginWithView.checkbox.isChecked() or \
                self.endWithView.checkbox.isChecked() or \
                self.allowCycleView.checkbox.isChecked():
            self.queryChainsAllButton.setDisabled(True)
            self.queryChainWordUniqueButton.setDisabled(True)
        else:
            self.queryChainsAllButton.setDisabled(False)
            self.queryChainWordUniqueButton.setDisabled(False)


class MainView(QWidget):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setWindowTitle('WordList')
        self.setMinimumSize(800, 500)
        layout = QHBoxLayout()
        self.ioView = IOView()
        layout.addWidget(self.ioView)
        line = QFrame()
        line.setFrameShape(QFrame.VLine)
        line.setFrameShadow(QFrame.Sunken)
        layout.addWidget(line)
        self.optionView = OptionView()
        layout.addWidget(self.optionView)
        self.optionView.queryChainsAllButton.clicked.connect(self.onQueryChainsAll)
        self.optionView.queryChainWordButton.clicked.connect(self.onQueryChainWord)
        self.optionView.queryChainCharButton.clicked.connect(self.onQueryChainChar)
        self.optionView.queryChainWordUniqueButton.clicked.connect(self.onQueryChainWordUnique)
        self.setLayout(layout)
        self.show()

    def warn(self, info):
        message = QMessageBox(self)
        message.setText("错误")
        message.setInformativeText(info)
        message.setIcon(QMessageBox.Critical)
        message.exec_()

    def onQueryChainsAll(self):
        words = lib.split_word(self.ioView.input)
        startTime = time.time()
        try:
            ans = core.gen_chains_all(words)
            if len(ans) <= 20000:
                text = str(len(ans)).encode() + b'\n' + b'\n'.join(ans)
            else:
                text = str(len(ans)).encode()
            self.setOutput(text)
            self.optionView.profileLabel.setText(f"计算用时：{round(time.time() - startTime, 5)}秒")
        except core.WordCycleException:
            self.warn("输入中存在单词环")
            self.optionView.profileLabel.setText("计算出现错误")

    def onQueryChainWord(self):
        words = lib.split_word(self.ioView.input)
        head = self.optionView.beginWithView.getCChar()
        tail = self.optionView.endWithView.getCChar()
        enable_loop = self.optionView.allowCycleView.checkbox.isChecked()
        startTime = time.time()
        try:
            ans = core.gen_chain_word(words, head, tail, enable_loop)
            text = str(len(ans)).encode() + b'\n' + b'\n'.join(ans)
            self.setOutput(text)
            self.optionView.profileLabel.setText(f"计算用时：{round(time.time() - startTime, 5)}秒")
        except core.WordCycleException:
            self.warn("输入中存在单词环")
            self.optionView.profileLabel.setText("计算出现错误")

    def onQueryChainChar(self):
        words = lib.split_word(self.ioView.input)
        head = self.optionView.beginWithView.getCChar()
        tail = self.optionView.endWithView.getCChar()
        enable_loop = self.optionView.allowCycleView.checkbox.isChecked()
        startTime = time.time()
        try:
            ans = core.gen_chain_char(words, head, tail, enable_loop)
            text = str(len(ans)).encode() + b'\n' + b'\n'.join(ans)
            self.setOutput(text)
            self.optionView.profileLabel.setText(f"计算用时：{round(time.time() - startTime, 5)}秒")
        except core.WordCycleException:
            self.warn("输入中存在单词环")
            self.optionView.profileLabel.setText("计算出现错误")

    def onQueryChainWordUnique(self):
        words = lib.split_word(self.ioView.input)
        startTime = time.time()
        try:
            ans = core.gen_chain_word_unique(words)
            text = str(len(ans)).encode() + b'\n' + b'\n'.join(ans)
            self.setOutput(text)
            self.optionView.profileLabel.setText(f"计算用时：{round(time.time() - startTime, 5)}秒")
        except core.WordCycleException:
            self.warn("输入中存在单词环")
            self.optionView.profileLabel.setText("计算出现错误")

    def setOutput(self, text: bytes):
        self.ioView.output = text
