#!/bin/bash

if ! [ -d ./build ]; then
    mkdir ./build
fi

pyuic5 ./mainwindow.ui -o ./build/mainwindow.py

python main.py