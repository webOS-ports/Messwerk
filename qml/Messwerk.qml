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

import Eos.Window 0.1
// Units & font sizes
import LunaNext.Common 0.1

WebOSWindow {
    id: appWindow

    width: Settings.displayWidth
    height: Settings.displayHeight
    visible: true
    windowType: "_WEBOS_WINDOW_TYPE_CARD"
    title: "Messwerk"

    // Behind the header; the pages paint their own gradient
    Rectangle {
        z: -1
        anchors.fill: parent
        color: "#D8D8D8"
    }

    AppHeader {
        id: header

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        // The settings apps' header height, less of it on a phone's width or
        // a short screen like the Q25's square
        height: (appWindow.height < Units.gu(70) || appWindow.width < Units.gu(50)) ? Units.gu(7.5) : Units.gu(10)

        title: pageStack.currentItem && pageStack.currentItem.title ? pageStack.currentItem.title : "Messwerk"
        actionComponent: pageStack.currentItem ? pageStack.currentItem.headerAction : null
    }

    StackView {
        id: pageStack

        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        focus: true

        // Pushed rather than given as initialItem: with the LuneOS style's
        // StackView a URL there is silently ignored and the card stays empty
        Component.onCompleted: push(Qt.resolvedUrl("pages/SensorListPage.qml"))

        // The back gesture arrives as Escape (see LunaNext.Common's EventType)
        Keys.onReleased: function(event) {
            if (event.key === Qt.Key_Escape && pageStack.depth > 1) {
                pageStack.pop();
                event.accepted = true;
            }
        }
    }
}
