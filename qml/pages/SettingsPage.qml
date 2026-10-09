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

BasePage {
    id: page

    title: "Settings"

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Logging"

            Column {
                width: parent.width

                LabelAndTextField {
                    width: parent.width
                    label: "Folder"
                    text: MesswerkSettings.loggingPath
                    inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
                    acceptable: text !== ""

                    // Saved as typed, the way the settings app does; a log
                    // only reads it when it starts
                    onEdited: function(newText) {
                        if (newText !== "")
                            MesswerkSettings.loggingPath = newText;
                    }
                }

                ExplanationText {
                    text: "Each log is a new CSV file in this folder. The folder is created when the first log starts."
                }
            }
        }
    }
}
