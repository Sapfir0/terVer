#!/bin/bash

if ! [ -d ./build ]; then
    mkdir ./build
fi

pyuic5 ./mainwindow.ui -o ./build/mainwindow.py
pyrcc5 -o mainwindow_rc.py mainwindow.qrc
python3 main.py #аааааааааааааааааааааааа
