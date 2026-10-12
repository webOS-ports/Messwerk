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

    title: "Pressure"
    explanation: PressureSensor.available ? "" : "This device has no pressure sensor."

    Component.onCompleted: PressureSensor.activate(Constants.PART_PAGE)
    Component.onDestruction: PressureSensor.deactivate(Constants.PART_PAGE)

    Connections {
        target: PressureSensor
        function onReadingUpdated() {
            pressureRow.addValue(PressureSensor.pressure);
        }
    }

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Air pressure"

            PlotRow {
                id: pressureRow
                label: "Pressure"
                value: PressureSensor.pressure.toFixed(1) + " Pa"
            }
        }

        LoggingGroup {
            sensor: PressureSensor
        }
    }
}
