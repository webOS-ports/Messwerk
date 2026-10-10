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

    title: "Position"
    explanation: PositionSensor.available ? "" : "No position source is available on this device."

    property bool decimalCoord: true

    function formatCoordinate(n, isLon) {
        if (!isFinite(n))
            return "–";

        if (decimalCoord)
            return n.toFixed(6) + " °";

        // Rounded once at the finest unit, so 59.9999" cannot show as 60"
        var totalSec = Math.round(Math.abs(n) * 3600);
        var deg = Math.floor(totalSec / 3600);
        var min = Math.floor(totalSec / 60) % 60;
        var sec = totalSec % 60;
        var hemisphere = isLon ? (n >= 0 ? "E" : "W") : (n >= 0 ? "N" : "S");

        return deg + "° " + min + "' " + sec + "\" " + hemisphere;
    }

    function formatMeters(n) {
        return (isFinite(n) && n >= 0) ? n.toFixed(2) + " m" : "–";
    }

    function formatAltitude(n) {
        return isFinite(n) ? n.toFixed(2) + " m" : "–";
    }

    function formatFix(n) {
        switch (n) {
        case Constants.COORD_2D:
            return "2D";
        case Constants.COORD_3D:
            return "3D";
        case Constants.COORD_INVALID:
            return "None";
        default:
            return "Unknown";
        }
    }

    Component.onCompleted: PositionSensor.activate(Constants.PART_PAGE)
    Component.onDestruction: PositionSensor.deactivate(Constants.PART_PAGE)

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Coordinates (WGS84)"

            Column {
                width: parent.width

                LabelAndValue {
                    width: parent.width
                    label: "Fix"
                    value: page.formatFix(PositionSensor.coordType)
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Latitude"
                    value: page.formatCoordinate(PositionSensor.latitude, false)
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Longitude"
                    value: page.formatCoordinate(PositionSensor.longitude, true)
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Altitude"
                    value: page.formatAltitude(PositionSensor.altitude)
                }
            }
        }

        GroupBox {
            width: parent.width
            title: "Accuracy"

            Column {
                width: parent.width

                LabelAndValue {
                    width: parent.width
                    label: "Horizontal"
                    value: page.formatMeters(PositionSensor.horzAccuracy)
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Vertical"
                    value: page.formatMeters(PositionSensor.vertAccuracy)
                }
            }
        }

        GroupBox {
            width: parent.width
            title: "Maidenhead Locator"

            LabelAndValue {
                width: parent.width
                label: "Grid"
                value: PositionSensor.maidenhead !== "" ? PositionSensor.maidenhead : "–"
            }
        }

        GroupBox {
            width: parent.width
            title: "Format"

            LabelAndSwitch {
                label: "Degrees, minutes, seconds"
                checked: !page.decimalCoord
                onToggled: page.decimalCoord = !checked
            }
        }

        LoggingGroup {
            sensor: PositionSensor
        }
    }
}
