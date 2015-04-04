# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-anr_companion

CONFIG += sailfishapp c++11
QT += sql network

SOURCES += src/harbour-anr_companion.cpp \
    src/downloadmanager.cpp \
    src/downloader.cpp \
    src/dbmanager.cpp \
    src/settings.cpp \
    src/card.cpp \
    src/numericfilter.cpp \
    src/categoryfilter.cpp \
    src/cardproxymodel.cpp \
    src/cardmodel.cpp \
    src/countcardmodel.cpp \
    src/deckcardmodel.cpp \
    src/deckmodel.cpp \
    src/deckcardproxymodel.cpp \
    src/deckproxymodel.cpp

OTHER_FILES += qml/harbour-anr_companion.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-anr_companion.changes.in \
    rpm/harbour-anr_companion.spec \
    rpm/harbour-anr_companion.yaml \
    translations/*.ts \
    harbour-anr_companion.desktop \
    qml/components/SearchHeader.qml \
    qml/components/OpenBrowser.qml \
    qml/components/GridContextMenu.qml \
    qml/components/CounterLabel.qml \
    qml/components/CardImage.qml \
    qml/pages/SearchPage.qml \
    qml/pages/MainPage.qml \
    qml/pages/CardPage.qml \
    qml/pages/DeckPage.qml \
    qml/pages/DeckListPage.qml \
    qml/components/CardText.qml \
    qml/pages/AboutPage.qml \
    qml/pages/DBPage.qml

# to disable building translations every time, comment out the
# following CONFIG line
#CONFIG += sailfishapp_i18n
#TRANSLATIONS += translations/harbour-anr_companion-de.ts

DEFINES += ANR_OCTGNID=\\\"0f38e453-26df-4c04-9d67-6d43de939c77\\\"

HEADERS += \
    src/downloadmanager.h \
    src/downloader.h \
    src/dbmanager.h \
    src/settings.h \
    src/card.h \
    src/numericfilter.h \
    src/categoryfilter.h \
    src/cardproxymodel.h \
    src/cardmodel.h \
    src/countcardmodel.h \
    src/deckcardmodel.h \
    src/deckmodel.h \
    src/deckcardproxymodel.h \
    src/deckproxymodel.h

