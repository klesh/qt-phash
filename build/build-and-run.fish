#!/usr/bin/env fish

g++ \
    -lX11 -lpthread \
    -DQT_WIDGETS_LIB -I/usr/include/qt/QtWidgets -I/usr/include/qt -I/usr/include/qt/QtCore -DQT_GUI_LIB -I/usr/include/qt/QtGui -DQT_CORE_LIB -lQt5Widgets -lQt5Gui -lQt5Core -fPIC -fopenmp \
    -o $argv[2] $argv[1] && $argv[2]