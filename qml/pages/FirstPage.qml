import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Controls.LuneOS 2.0

import LunaNext.Common 0.1

import "CustomTheme"

BasePage {
    id: page
    headerText: "Select Sensor"

    function instantiatePage(subPage)
    {
        // remove any previously instantiated page
        var previousPage = swipeView.itemAt(1);
        if(!!previousPage) swipeView.removeItem(previousPage);

        var newComponent = Qt.createComponent(subPage);
        var newPage = newComponent.createObject(swipeView);
        swipeView.currentIndex = 1;
    }

    // To enable PullDownMenu, place our content in a SilicaFlickable
    Flickable {
        anchors.fill: parent
/*
        // PullDownMenu and PushUpMenu must be declared in SilicaFlickable, SilicaListView or SilicaGridView
        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsDialog.qml"))
            }
            MenuItem {
                text: qsTr("Info")
                onClicked: pageStack.push(Qt.resolvedUrl("InfoPage.qml"))
            }
        }
*/
        contentHeight: column.height
        // cut out-of-bound graphics
        clip: true

        // Place our content in a Column.  The PageHeader is always placed at the top
        // of the page, followed by our content.
        Column {
            id: column

            width: page.width
            spacing: Units.gu(2.0)

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Position")
                onClicked: instantiatePage(Qt.resolvedUrl("PositionPage.qml"))
                highlighted: satelliteinfo.isLogging
                font.pixelSize: FontUtils.sizeToPixels("large")
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("GNSS Satellites")
                onClicked: instantiatePage(Qt.resolvedUrl("SatellitePage.qml"))
                highlighted: satelliteinfo.isLogging
                font.pixelSize: FontUtils.sizeToPixels("large")
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Rotation")
                onClicked: instantiatePage(Qt.resolvedUrl("RotationPage.qml"))
                highlighted: rotationsensor.isLogging
                font.pixelSize: FontUtils.sizeToPixels("large")
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Accelerometer")
                onClicked: instantiatePage(Qt.resolvedUrl("AccelPage.qml"))
                highlighted: accelerometer.isLogging
                font.pixelSize: FontUtils.sizeToPixels("large")
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Gyroscope")
                onClicked: instantiatePage(Qt.resolvedUrl("GyroPage.qml"))
                highlighted: gyroscope.isLogging
                font.pixelSize: FontUtils.sizeToPixels("large")
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Magnetometer")
                onClicked: instantiatePage(Qt.resolvedUrl("MagnetPage.qml"))
                highlighted: magnetometer.isLogging
                font.pixelSize: FontUtils.sizeToPixels("large")
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Light & Proximity")
                onClicked: instantiatePage(Qt.resolvedUrl("LightPage.qml"))
                highlighted: (lightsensor.isLogging || proximitysensor.isLogging)
                font.pixelSize: FontUtils.sizeToPixels("large")
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Pressure Sensor")
                onClicked: instantiatePage(Qt.resolvedUrl("PressurePage.qml"))
                highlighted: pressuresensor.isLogging
                font.pixelSize: FontUtils.sizeToPixels("large")
            }

            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                bottomPadding: Units.gu(2.0)
                spacing: Units.gu(3.0)
                Button {
                    id: settingsButton
                    text: qsTr("Settings")
                    onClicked: instantiatePage(Qt.resolvedUrl("SettingsDialog.qml"))
                    font.pixelSize: FontUtils.sizeToPixels("large")

                    LuneOSButton.mainColor: "orange"
                }
                Button {
                    id: infoButton
                    text: qsTr("About")
                    onClicked: instantiatePage(Qt.resolvedUrl("InfoPage.qml"))
                    font.pixelSize: settingsButton.font.pixelSize

                    LuneOSButton.mainColor: LuneOSButton.blueColor
                }
            }
        }
    }
}


