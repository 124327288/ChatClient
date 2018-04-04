#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T16:57:40
#
#-------------------------------------------------

QT       += core gui network sql
QT       += webenginewidgets

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
        luatool.cpp \
        mainwindow.cpp \
        program.cpp \
        chatwindow.cpp \
        stdafx.cpp \
        pdfwindow.cpp \
        settingdialog.cpp \
        sqlitetool.cpp \
        signal.cpp \
        emotiondialog.cpp \
        screenshot.cpp  \
        Protocol/protocol.cpp \
        Protocol/tcpsocket.cpp \
        Protocol/C2S/c2sprotocol.cpp \
        Protocol/C2S/userpwdprotocol.cpp \
        Protocol/S2C/loginresprotocol.cpp \
        Protocol/S2C/signatureprotocol.cpp \
        Model/friendlistmodel.cpp \
        Model/sessionlistmodel.cpp \
        View/friendlistview.cpp \
        Delegate/friendlistitemdelegate.cpp \
        Delegate/sessionlistitemdelegate.cpp \
        Delegate/emotionsitemdelegate.cpp \
        Tim/timcallback.cpp \
        Tim/timtool.cpp \
    filewindow.cpp \
    Model/filetablemodel.cpp \
    Delegate/filetableitemdelegate.cpp \
    webconnect.cpp \
    databasetool.cpp \
    Table/id.cpp \
    Table/account.cpp \
    Table/sig.cpp

HEADERS += \
        loginwindow.h \
        luatool.h \
        mainwindow.h \
        exptype.h \
        expdef.h \
        program.h \
        chatwindow.h \
        stdafx.h \
        pdfwindow.h \
        settingdialog.h \
        sqlitetool.h \
        signal.h \
        emotiondialog.h \
        screenshot.h    \
        Protocol/protocol.h \
        Protocol/tcpsocket.h \
        Protocol/C2S/c2sprotocol.h \
        Protocol/C2S/userpwdprotocol.h \
        Protocol/S2C/loginresprotocol.h \
        Protocol/S2C/s2cprotocol.h \
        Protocol/S2C/signatureprotocol.h \
        Model/friendlistmodel.h \
        Model/sessionlistmodel.h \
        View/friendlistview.h \
        Delegate/friendlistitemdelegate.h \
        Delegate/sessionlistitemdelegate.h \
        Delegate/emotionsitemdelegate.h \
        Tim/timcallback.h \
        Tim/timtool.h \
    filewindow.h \
    Model/filetablemodel.h \
    Delegate/filetableitemdelegate.h \
    webconnect.h \
    Tim/tim_header.h \
    databasetool.h \
    Table/id.h \
    Table/table.h \
    Table/account.h \
    Table/sig.h

FORMS += \
        loginwindow.ui \
        mainwindow.ui \
        chatwindow.ui \
        pdfwindow.ui

RESOURCES += \
    img.qrc \
    language.qrc

# IM_Windows_SDK 2.5.6

#INCLUDEPATH += $$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/c_includes
#DEPENDPATH += $$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/c_includes

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/libs/release/ -llibtim
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/IM_Windows_SDK_2.5.6/libs/libs/debug/ -llibtim

# IM_Windows_SDK 2.5.7_20180321

INCLUDEPATH += $$PWD/thirdparty/windows_v2.5.7_20180321/c_includes
DEPENDPATH += $$PWD/thirdparty/windows_v2.5.7_20180321/c_includes

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/thirdparty/windows_v2.5.7_20180321/libs/release/ -llibtim
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/thirdparty/windows_v2.5.7_20180321/libs/debug/ -llibtim

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
