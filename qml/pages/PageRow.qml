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
import QtQuick.Layouts 1.3

// Units & font sizes
import LunaNext.Common 0.1
// ListDelegateSeparator
import LuneOS.Components 1.0

/*
 * One row of the start page: a page's title, and on the right whether it is
 * logging or, when the device lacks every sensor it shows, that it has none.
 */
ItemDelegate {
    id: row

    property var entry: ({ "title": "", "page": "", "sensors": [] })
    property int rowIndex: 0
    property int rowCount: 1

    readonly property bool logging: {
        for (var i = 0; i < entry.sensors.length; i++) {
            if (entry.sensors[i].isLogging)
                return true;
        }
        return false;
    }

    readonly property bool available: {
        if (entry.sensors.length === 0)
            return true;
        for (var i = 0; i < entry.sensors.length; i++) {
            if (entry.sensors[i].available)
                return true;
        }
        return false;
    }

    height: Units.gu(6)

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Units.gu(1)
        anchors.rightMargin: Units.gu(1)
        spacing: Units.gu(1)

        Label {
            Layout.fillWidth: true
            text: row.entry.title
            elide: Text.ElideRight
            color: row.available ? "#2a2929" : "#888888"
            font.pixelSize: FontUtils.sizeToPixels("medium")
        }

        Label {
            visible: text !== ""
            text: row.logging ? "Logging" : (row.available ? "" : "Not available")
            color: row.logging ? "#be0003" : "#555555"
            font.pixelSize: FontUtils.sizeToPixels("small")
            font.bold: row.logging
        }
    }

    ListDelegateSeparator {
        anchors.fill: parent
        index: row.rowIndex
        count: row.rowCount
    }
}
