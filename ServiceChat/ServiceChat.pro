#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T16:57:40
#
#-------------------------------------------------

QT       += core gui
QT       += network

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER  = stdafx.h

QMAKE_CXXFLAGS += /MP

TRANSLATIONS += client_en.ts
TRANSLATIONS += client_cn.ts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServiceChat
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        loginwindow.cpp \
    timtool.cpp \
    luatool.cpp \
    mainwindow.cpp \
    protocol.cpp \
    S2C/signatureprotocol.cpp \
    tcpsocket.cpp \
    program.cpp \
    S2C/loginresprotocol.cpp \
    C2S/userpwdprotocol.cpp \
    C2S/c2sprotocol.cpp \
    chatwindow.cpp \
    linkmanlistmodel.cpp \
    timcallback.cpp \
    stdafx.cpp \
    pdfwindow.cpp

HEADERS += \
        loginwindow.h \
    timtool.h \
    luatool.h \
    mainwindow.h \
    exptype.h \
    protocol.h \
    C2S/c2sprotocol.h \
    S2C/signatureprotocol.h \
    S2C/s2cprotocol.h \
    expdef.h \
    C2S/userpwdprotocol.h \
    tcpsocket.h \
    program.h \
    S2C/loginresprotocol.h \
    chatwindow.h \
    linkmanlistmodel.h \
    timcallback.h \
    stdafx.h \
    tim.h \
    pdfwindow.h

FORMS += \
        loginwindow.ui \
    mainwindow.ui \
    chatwindow.ui \
    pdfwindow.ui

RESOURCES += \
    res.qrc

# IM_Windows_SDK

INCLUDEPATH += $$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/c_includes
DEPENDPATH += $$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/c_includes

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/libs/release/ -llibtim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/libs/debug/ -llibtim

# Lua5.1

INCLUDEPATH += $$PWD/thirdparty/Lua/include
DEPENDPATH += $$PWD/thirdparty/Lua/include

win32: LIBS += -L$$PWD/thirdparty/Lua/lib/ -llua5.1
win32: LIBS += -L$$PWD/thirdparty/Lua/lib/ -llua51

# MuPdf

INCLUDEPATH += $$PWD/thirdparty/mupdf/include
DEPENDPATH += $$PWD/thirdparty/mupdf/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/mupdf/lib/release/ -llibmupdf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/mupdf/lib/debug/ -llibmupdf

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/mupdf/lib/release/libmupdf.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/mupdf/lib/debug/libmupdf.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/mupdf/lib/release/ -llibresources
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/mupdf/lib/debug/ -llibresources

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/mupdf/lib/release/libresources.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/mupdf/lib/debug/libresources.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/mupdf/lib/release/ -llibthirdparty
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/mupdf/lib/debug/ -llibthirdparty

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/mupdf/lib/release/libthirdparty.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/thirdparty/mupdf/lib/debug/libthirdparty.lib
