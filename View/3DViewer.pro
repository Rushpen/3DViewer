QT += core gui widgets opengl sql

CONFIG += c++17

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Control/UIControl.cpp \
    Scene/UIScene.cpp \
    Scene/openGL/OpenGLWidget.cpp \
    Menu/UIMenubar.cpp \
    Menu/UIMyDialog.cpp \
    Record/Screen.cpp \
    Record/ScreenshotDialog.cpp \
    ../3DViewer_back/back/*.cpp \
    ../3DViewer_back/mc/*.cpp \
    ../matrix/s21_matrix_oop.cpp \
    ../database/settingsManager.cpp

HEADERS += \
    MainWindow.h \
    Control/UIControl.h \
    Scene/UIScene.h \
    Scene/openGL/OpenGLWidget.h \
    Menu/UIMenubar.h \
    Menu/UIMyDialog.h \
    Record/Screen.h \
    Record/ScreenshotDialog.h \
    ../3DViewer_back/back/*.h \
    ../3DViewer_back/mc/*.h \
    ../matrix/s21_matrix_oop.h \
    ../database/settingsManager.h \

LIBS += -lGLU -lGL

include(Record/QtGifImageLib/src/gifimage/qtgifimage.pri)

