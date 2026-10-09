#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QTimer>
#include <QUrl>
#include <QQmlEngine>

#include "accelerometer.h"
#include "gyroscope.h"
#include "magnetometer.h"
#include "rotation.h"
#include "light.h"
#include "pressuresensor.h"
#include "proximity.h"
#include "satelliteinfo.h"
#include "position.h"

#include "settings.h"

#include "plotwidget.h"
#include "satelliteposwidget.h"
#include "satellitestrengthwidget.h"


int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_CONTROLS_STYLE", "QtQuick.Controls.LuneOS");

    QGuiApplication app(argc, argv);

    QString qml = QStringLiteral("%1/qml/Messwerk.qml").arg(QCoreApplication::applicationDirPath());

    qmlRegisterType<PlotWidget>("harbour.messwerk.MesswerkWidgets", 1, 0, "PlotWidget");
    qmlRegisterType<SatellitePosWidget>("harbour.messwerk.MesswerkWidgets", 1, 0, "SatellitePosWidget");
    qmlRegisterType<SatelliteStrengthWidget>("harbour.messwerk.MesswerkWidgets", 1, 0, "SatelliteStrengthWidget");

    /*
     * Everything QML talks to is declared before the engine, so it is
     * destroyed after it. The pages deactivate their sensors from
     * Component.onDestruction, and the engine runs those while it is torn
     * down: with the engine declared first, as it used to be, they ran
     * against sensors that had already been destroyed.
     */
    Accelerometer accelerometer(false);
    Gyroscope gyroscope(false);
    Magnetometer magnetometer(false);
    Rotation rotation(false);
    Light light(false);
    PressureSensor pressuresensor(false);
    Proximity proximity(true);
    SatelliteInfo satelliteinfo;
    Position position;

    // connect not self-refreshing sensors to the global timer
    QTimer refreshTimer;

    for(Sensor *sensor: std::initializer_list<Sensor*>{&accelerometer, &pressuresensor, &gyroscope,
                                                       &magnetometer, &rotation, &light}) {
        QObject::connect(&refreshTimer, &QTimer::timeout, sensor, &Sensor::refresh);
    }

    // The engine does not take ownership of these, see above for their lifetime
    const char *uri = "harbour.messwerk.Sensors";
    qmlRegisterSingletonInstance(uri, 1, 0, "Accelerometer", &accelerometer);
    qmlRegisterSingletonInstance(uri, 1, 0, "Gyroscope", &gyroscope);
    qmlRegisterSingletonInstance(uri, 1, 0, "Magnetometer", &magnetometer);
    qmlRegisterSingletonInstance(uri, 1, 0, "RotationSensor", &rotation);
    qmlRegisterSingletonInstance(uri, 1, 0, "LightSensor", &light);
    qmlRegisterSingletonInstance(uri, 1, 0, "PressureSensor", &pressuresensor);
    qmlRegisterSingletonInstance(uri, 1, 0, "ProximitySensor", &proximity);
    qmlRegisterSingletonInstance(uri, 1, 0, "SatelliteInfo", &satelliteinfo);
    qmlRegisterSingletonInstance(uri, 1, 0, "PositionSensor", &position);
    qmlRegisterSingletonInstance(uri, 1, 0, "MesswerkSettings", &Settings::instance());

    QQmlApplicationEngine engine;

    engine.load(QUrl::fromLocalFile(qml));

    if(engine.rootObjects().isEmpty()) {
        return 1;
    }

    refreshTimer.start(100);

    return app.exec();
}
