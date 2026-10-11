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
import LuneOS.Forms 1.0

/*
 * Recording a sensor to a CSV file: one switch, and a line saying where the
 * file goes, where it went, or why it could not be created.
 *
 * "sensor" is any of the Sensors singletons; they all have isLogging,
 * logFileName, startLogging(), stopLogging() and loggingFailed().
 */
GroupBox {
    id: loggingGroup

    property QtObject sensor: null
    property string switchLabel: "Record to file"

    property string _error: ""

    title: "Logging"
    width: parent ? parent.width : implicitWidth

    Connections {
        target: loggingGroup.sensor
        function onLoggingFailed(errorString) {
            loggingGroup._error = errorString;
        }
        function onIsLoggingChanged() {
            if (loggingGroup.sensor.isLogging)
                loggingGroup._error = "";
        }
    }

    Column {
        width: parent.width

        LabelAndSwitch {
            label: loggingGroup.switchLabel
            enabled: loggingGroup.sensor !== null && loggingGroup.sensor.available
            checked: loggingGroup.sensor !== null && loggingGroup.sensor.isLogging

            onToggled: {
                if (checked)
                    loggingGroup.sensor.startLogging();
                else
                    loggingGroup.sensor.stopLogging();

                // Toggling broke the binding; the sensor has the final say,
                // since starting can fail
                checked = Qt.binding(function() {
                    return loggingGroup.sensor !== null && loggingGroup.sensor.isLogging;
                });
            }
        }

        ExplanationText {
            text: {
                if (loggingGroup._error !== "")
                    return loggingGroup._error;
                if (loggingGroup.sensor && loggingGroup.sensor.isLogging)
                    return "Writing to " + loggingGroup.sensor.logFileName + ". Logging carries on after you leave this page.";
                return "Writes the values to a new CSV file in " + MesswerkSettings.loggingPath + ".";
            }
            color: loggingGroup._error !== "" ? "#be0003" : "#666666"
        }
    }
}
