pyuic5 ./mainwindow.ui -o ./build/mainwindow.py
pyrcc5 -o mainwindow_rc.py mainwindow.qrc
python main.py