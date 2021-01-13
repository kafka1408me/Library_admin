QT       += core gui websockets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authorization.cpp \
    connector.cpp \
    dialog_addbook.cpp \
    dialog_addclient.cpp \
    dialog_editbook.cpp \
    dialog_editclient.cpp \
    main.cpp \
    mainwindow.cpp \ #\
    mydelegateforlistview.cpp \
    tablebooks.cpp \
    tableusers.cpp
#    tablemodelBooks.cpp \
#    tablemodelClients.cpp

HEADERS += \
  ../ServerDB111/codes.h \
    authorization.h \
    codes.h \
    connector.h \
    dialog_addbook.h \
    dialog_addclient.h \
    dialog_editbook.h \
    dialog_editclient.h \
    mainwindow.h \ #\
  mydelegateforlistview.h \
  tablebooks.h \
  tableusers.h
#    tablemodelBooks.h \
#    tablemodelClients.h

FORMS += \
    authorization.ui \
    dialog_addbook.ui \
    dialog_addclient.ui \
    dialog_editbook.ui \
    dialog_editclient.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
