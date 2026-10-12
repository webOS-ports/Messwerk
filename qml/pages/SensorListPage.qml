/*
 * (c) 2026 Herman van Hazendonk <github.com@herrie.org>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.9
import QtQuick.Controls 2.2

import harbour.messwerk.Sensors 1.0

import "../Common"
import LuneOS.Forms 1.0

AppPage {
    id: page

    title: "Messwerk"

    // "sensors" are what a page shows; its row is marked while any of them logs
    readonly property var sensorPages: [
        { "title": "Position",          "page": "PositionPage.qml",  "sensors": [PositionSensor] },
        { "title": "GNSS Satellites",   "page": "SatellitePage.qml", "sensors": [SatelliteInfo] },
        { "title": "Rotation",          "page": "RotationPage.qml",  "sensors": [RotationSensor] },
        { "title": "Accelerometer",     "page": "AccelPage.qml",     "sensors": [Accelerometer] },
        { "title": "Gyroscope",         "page": "GyroPage.qml",      "sensors": [Gyroscope] },
        { "title": "Magnetometer",      "page": "MagnetPage.qml",    "sensors": [Magnetometer] },
        { "title": "Light & Proximity", "page": "LightPage.qml",     "sensors": [LightSensor, ProximitySensor] },
        { "title": "Pressure",          "page": "PressurePage.qml",  "sensors": [PressureSensor] }
    ]

    readonly property var appPages: [
        { "title": "Settings", "page": "SettingsPage.qml", "sensors": [] },
        { "title": "About",    "page": "AboutPage.qml",    "sensors": [] }
    ]

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Sensors"

            Column {
                width: parent.width

                Repeater {
                    model: page.sensorPages
                    delegate: PageRow {
                        width: parent.width
                        entry: modelData
                        rowIndex: index
                        rowCount: page.sensorPages.length
                        onClicked: page.StackView.view.push(Qt.resolvedUrl(entry.page))
                    }
                }
            }
        }

        GroupBox {
            width: parent.width
            title: "Messwerk"

            Column {
                width: parent.width

                Repeater {
                    model: page.appPages
                    delegate: PageRow {
                        width: parent.width
                        entry: modelData
                        rowIndex: index
                        rowCount: page.appPages.length
                        onClicked: page.StackView.view.push(Qt.resolvedUrl(entry.page))
                    }
                }
            }
        }
    }
}
