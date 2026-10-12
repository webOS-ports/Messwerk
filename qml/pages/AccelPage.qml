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
import LuneOS.Forms 1.0

AppPage {
    id: page

    title: "Accelerometer"
    explanation: Accelerometer.available ? "" : "This device has no accelerometer."

    readonly property real standardGravity: 9.80665

    property bool useGs: false

    function convertNumber(n) {
        return useGs ? n / standardGravity : n;
    }

    function formatNumber(n) {
        return convertNumber(n).toFixed(3) + (useGs ? " g" : " m/s²");
    }

    Component.onCompleted: Accelerometer.activate(Constants.PART_PAGE)
    Component.onDestruction: Accelerometer.deactivate(Constants.PART_PAGE)

    Connections {
        target: Accelerometer
        function onReadingUpdated() {
            xRow.addValue(page.convertNumber(Accelerometer.ax));
            yRow.addValue(page.convertNumber(Accelerometer.ay));
            zRow.addValue(page.convertNumber(Accelerometer.az));
        }
    }

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Acceleration"

            Column {
                width: parent.width

                PlotRow {
                    id: xRow
                    label: "X"
                    value: page.formatNumber(Accelerometer.ax)
                }
                PlotRow {
                    id: yRow
                    label: "Y"
                    value: page.formatNumber(Accelerometer.ay)
                }
                PlotRow {
                    id: zRow
                    label: "Z"
                    value: page.formatNumber(Accelerometer.az)
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Absolute"
                    value: page.formatNumber(Accelerometer.abs)
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Without gravity"
                    value: page.formatNumber(Accelerometer.abs - page.standardGravity)
                }
            }
        }

        GroupBox {
            width: parent.width
            title: "Units"

            LabelAndSwitch {
                label: "Show in g"
                checked: page.useGs
                onToggled: {
                    page.useGs = checked;
                    xRow.reset();
                    yRow.reset();
                    zRow.reset();
                }
            }
        }

        LoggingGroup {
            sensor: Accelerometer
        }
    }
}
