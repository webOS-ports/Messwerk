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
TARGET = harbour-messwerk

QT += sensors positioning gui qml quick

CONFIG += c++17

SOURCES += src/Messwerk.cpp \
    src/accelerometer.cpp \
    src/gyroscope.cpp \
    src/magnetometer.cpp \
    src/sensor.cpp \
    src/plotwidget.cpp \
    src/rotation.cpp \
    src/light.cpp \
    src/proximity.cpp \
    src/pressuresensor.cpp \
    src/satelliteposwidget.cpp \
    src/satelliteinfo.cpp \
    src/activateable.cpp \
    src/satellitestrengthwidget.cpp \
    src/settings.cpp \
    src/position.cpp \
    src/wakelock.cpp \
    src/logfile.cpp

QML_FILES = qml/Messwerk.qml \
    qml/Constants.js \
    qml/Common/BasePage.qml \
    qml/Common/LoggingGroup.qml \
    qml/Common/PlotRow.qml \
    qml/pages/SensorListPage.qml \
    qml/pages/PageRow.qml \
    qml/pages/PositionPage.qml \
    qml/pages/SatellitePage.qml \
    qml/pages/RotationPage.qml \
    qml/pages/AccelPage.qml \
    qml/pages/GyroPage.qml \
    qml/pages/MagnetPage.qml \
    qml/pages/LightPage.qml \
    qml/pages/PressurePage.qml \
    qml/pages/SettingsPage.qml \
    qml/pages/AboutPage.qml

RUNTIME_FILES = appinfo.json \
                harbour-messwerk.png

OTHER_FILES += translations/*.ts $$QML_FILES $$RUNTIME_FILES

qml_files.path = $$DEPLOYMENT_PATH
qml_files.files = qml

runtime_files.path = $$DEPLOYMENT_PATH
runtime_files.files = $$RUNTIME_FILES

target.path = $$DEPLOYMENT_PATH
INSTALLS += target qml_files runtime_files

TRANSLATIONS += translations/harbour-messwerk-de.ts \
    translations/harbour-messwerk-pl.ts \
    translations/harbour-messwerk-sv.ts

HEADERS += \
    src/accelerometer.h \
    src/gyroscope.h \
    src/magnetometer.h \
    src/sensor.h \
    src/plotwidget.h \
    src/rotation.h \
    src/light.h \
    src/pressuresensor.h \
    src/proximity.h \
    src/satelliteposwidget.h \
    src/satelliteinfo.h \
    src/activateable.h \
    src/satellitestrengthwidget.h \
    src/settings.h \
    src/position.h \
    src/wakelock.h \
    src/logfile.h


