QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += sdk_no_version_check

ICON = icons/haushaltsbuch_mac.icns
RC_ICONS = icons/haushaltsbuch_win.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    sources/addtransactionview.cpp \
    sources/adminregistrationview.cpp \
    sources/applicationview.cpp \
    sources/categorydao.cpp \
    sources/catview.cpp \
    sources/changeinfoview.cpp \
    sources/changepasswordview.cpp \
    sources/changetransactionview.cpp \
    sources/createuserview.cpp \
    sources/dbmanager.cpp \
    sources/hashing.cpp \
    sources/inputcheck.cpp \
    sources/loginview.cpp \
    sources/main.cpp \
    sources/manageuserview.cpp \
    sources/menuview.cpp \
    sources/nopasswordloginview.cpp \
    sources/numberformatdelegate.cpp \
    sources/payoptiondao.cpp \
    sources/payoptionsview.cpp \
    sources/popupwidget.cpp \
    sources/transactiondao.cpp \
    sources/user.cpp \
    sources/userdao.cpp

HEADERS += \
    headers/addtransactionview.h \
    headers/adminregistrationview.h \
    headers/applicationview.h \
    headers/categorydao.h \
    headers/catview.h \
    headers/changeinfoview.h \
    headers/changepasswordview.h \
    headers/changetransactionview.h \
    headers/createuserview.h \
    headers/dbmanager.h \
    headers/global.h \
    headers/hashing.h \
    headers/inputcheck.h \
    headers/loginview.h \
    headers/manageuserview.h \
    headers/menuview.h \
    headers/nopasswordloginview.h \
    headers/numberformatdelegate.h \
    headers/payoptiondao.h \
    headers/payoptionsview.h \
    headers/popupwidget.h \
    headers/transactiondao.h \
    headers/user.h \
    headers/userdao.h

FORMS += \
    forms/addtransactionview.ui \
    forms/adminregistrationview.ui \
    forms/applicationview.ui \
    forms/catview.ui \
    forms/changeinfoview.ui \
    forms/changepasswordview.ui \
    forms/changetransactionview.ui \
    forms/createuserview.ui \
    forms/loginview.ui \
    forms/manageuserview.ui \
    forms/menuview.ui \
    forms/nopasswordloginview.ui \
    forms/payoptionsview.ui

TRANSLATIONS += \
    translations/Haushaltsbuch_de_DE.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH +=$$PWD/sources
INCLUDEPATH +=$$PWD/headers
INCLUDEPATH +=$$PWD/forms
INCLUDEPATH +=$$PWD/translations
