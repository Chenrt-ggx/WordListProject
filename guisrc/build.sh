python -m venv venv
source venv/Scripts/activate
pip install -r requirements.txt -i https://pypi.tuna.tsinghua.edu.cn/simple
pip install pyinstaller -i https://pypi.tuna.tsinghua.edu.cn/simple
pyinstaller.exe -F -w --path venv/Lib/site-packages/PyQt5/Qt5/bin/ main.py
cp core.dll dist