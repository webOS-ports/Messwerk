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

// Units
import LunaNext.Common 0.1

import harbour.messwerk.MesswerkWidgets 1.0
import harbour.messwerk.Sensors 1.0

import "../Constants.js" as Constants
import "../Common"

BasePage {
    id: page

    title: "GNSS Satellites"
    explanation: SatelliteInfo.available ? "Blue satellites are used for the fix. The red spoke points north."
                                         : "No satellite source is available on this device."

    // The rotation sensor turns the sky plot so its north follows the device
    Component.onCompleted: {
        SatelliteInfo.activate(Constants.PART_PAGE);
        RotationSensor.activate(Constants.PART_PAGE);
    }
    Component.onDestruction: {
        SatelliteInfo.deactivate(Constants.PART_PAGE);
        RotationSensor.deactivate(Constants.PART_PAGE);
    }

    SettingsPageContent {
        id: content

        GroupBox {
            width: parent.width
            title: "Sky"

            Column {
                width: parent.width

                SatellitePosWidget {
                    width: Math.min(parent.width, Units.gu(40))
                    height: width
                    anchors.horizontalCenter: parent.horizontalCenter

                    satelliteInfo: SatelliteInfo
                    northDirection: RotationSensor.rz

                    visibleColor: "#4b4b4b"
                    usedColor: "#2071bb"
                    scaleColor: "#ADADAD"
                    northColor: "#be0003"
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "In view"
                    value: SatelliteInfo.numberOfSatellites
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Used for the fix"
                    value: SatelliteInfo.numberOfUsedSatellites
                }
            }
        }

        GroupBox {
            width: parent.width
            title: "Signal strength"

            SatelliteStrengthWidget {
                width: parent.width
                height: Units.gu(20)

                satelliteInfo: SatelliteInfo

                visibleColor: "#4b4b4b"
                usedColor: "#2071bb"
                scaleColor: "#ADADAD"
            }
        }

        LoggingGroup {
            sensor: SatelliteInfo
        }
    }
}
