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

    title: "Magnetometer"
    explanation: Magnetometer.available ? "" : "This device has no magnetometer."

    // QMagnetometer reports Tesla
    function formatNumber(n) {
        return (n * 1e3).toFixed(4) + " mT";
    }

    Component.onCompleted: Magnetometer.activate(Constants.PART_PAGE)
    Component.onDestruction: Magnetometer.deactivate(Constants.PART_PAGE)

    Connections {
        target: Magnetometer
        function onReadingUpdated() {
            xRow.addValue(Magnetometer.mx);
            yRow.addValue(Magnetometer.my);
            zRow.addValue(Magnetometer.mz);
        }
    }

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Magnetic flux density"

            Column {
                width: parent.width

                PlotRow {
                    id: xRow
                    label: "X"
                    value: page.formatNumber(Magnetometer.mx)
                }
                PlotRow {
                    id: yRow
                    label: "Y"
                    value: page.formatNumber(Magnetometer.my)
                }
                PlotRow {
                    id: zRow
                    label: "Z"
                    value: page.formatNumber(Magnetometer.mz)
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Calibration"
                    value: (Magnetometer.precision * 100).toFixed(0) + " %"
                }
            }
        }

        LoggingGroup {
            sensor: Magnetometer
        }
    }
}
