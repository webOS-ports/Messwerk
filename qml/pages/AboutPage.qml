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

// Theme specific properties
import QtQuick.Controls.LuneOS 2.0

import "../Common"
import LuneOS.Forms 1.0

AppPage {
    id: page

    title: "About"

    SettingsPageContent {
        GroupBox {
            width: parent.width
            title: "Messwerk"

            Column {
                width: parent.width
                spacing: 8

                ExplanationText {
                    text: "Created by bytewerk. Send beer, pizza and Club Mate to the bytewerk hackerspace."
                }

                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "www.bytewerk.org"
                    LuneOSButton.mainColor: LuneOSButton.blueColor
                    onClicked: Qt.openUrlExternally("http://www.bytewerk.org")
                }

                Button {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "Source code"
                    LuneOSButton.mainColor: LuneOSButton.secondaryColor
                    onClicked: Qt.openUrlExternally("https://github.com/webOS-ports/Messwerk")
                }
            }
        }

        GroupBox {
            width: parent.width
            title: "Credits"

            Column {
                width: parent.width

                LabelAndValue {
                    width: parent.width
                    label: "Coding"
                    value: "cfr34k"
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "LuneOS port"
                    value: "Tofe"
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Quality assurance"
                    value: "sqozz"
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Art & Design"
                    value: "sqozz, cfr34k"
                }
                HorizontalSeparator {
                    width: parent.width
                }
                LabelAndValue {
                    width: parent.width
                    label: "Translations"
                    value: "sqozz, cfr34k"
                }
            }
        }
    }
}
