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

import "../Constants.js" as Constants
import "../Common"

BasePage {
    id: page

    title: "Rotation"
    explanation: RotationSensor.available ? "" : "This device has no rotation sensor."

    property bool useRad: false

    function convertNumber(n) {
        return useRad ? n * Math.PI / 180 : n;
    }

    function formatNumber(n) {
        return convertNumber(n).toFixed(3) + (useRad ? " rad" : " °");
    }

    Component.onCompleted: RotationSensor.activate(Constants.PART_PAGE)
    Component.onDestruction: RotationSensor.deactivate(Constants.PART_PAGE)

    Connections {
        target: RotationSensor
        function onReadingUpdated() {
            xRow.addValue(page.convertNumber(RotationSensor.rx));
            yRow.addValue(page.convertNumber(RotationSensor.ry));
            zRow.addValue(page.convertNumber(RotationSensor.rz));
        }
    }

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Orientation"

            Column {
                width: parent.width

                PlotRow {
                    id: xRow
                    label: "X"
                    value: page.formatNumber(RotationSensor.rx)
                }
                PlotRow {
                    id: yRow
                    label: "Y"
                    value: page.formatNumber(RotationSensor.ry)
                }
                PlotRow {
                    id: zRow
                    label: "Z"
                    value: page.formatNumber(RotationSensor.rz)
                }
            }
        }

        GroupBox {
            width: parent.width
            title: "Units"

            LabelAndSwitch {
                label: "Show in radians"
                checked: page.useRad
                onToggled: {
                    page.useRad = checked;
                    xRow.reset();
                    yRow.reset();
                    zRow.reset();
                }
            }
        }

        LoggingGroup {
            sensor: RotationSensor
        }
    }
}
