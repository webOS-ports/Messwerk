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

    title: "Light & Proximity"
    explanation: {
        if (!LightSensor.available && !ProximitySensor.available)
            return "This device has neither a light nor a proximity sensor.";
        if (!LightSensor.available)
            return "This device has no light sensor.";
        if (!ProximitySensor.available)
            return "This device has no proximity sensor.";
        return "";
    }

    Component.onCompleted: {
        LightSensor.activate(Constants.PART_PAGE);
        ProximitySensor.activate(Constants.PART_PAGE);
    }
    Component.onDestruction: {
        LightSensor.deactivate(Constants.PART_PAGE);
        ProximitySensor.deactivate(Constants.PART_PAGE);
    }

    Connections {
        target: LightSensor
        function onReadingUpdated() {
            brightnessRow.addValue(LightSensor.brightness);
        }
    }

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Light"

            PlotRow {
                id: brightnessRow
                label: "Illuminance"
                value: LightSensor.brightness.toFixed(1) + " lx"
            }
        }

        GroupBox {
            width: parent.width
            title: "Proximity"

            LabelAndValue {
                width: parent.width
                label: "Object near the screen"
                value: ProximitySensor.detected ? "Yes" : "No"
            }
        }

        LoggingGroup {
            title: "Light sensor logging"
            sensor: LightSensor
        }

        LoggingGroup {
            title: "Proximity sensor logging"
            sensor: ProximitySensor
        }
    }
}
