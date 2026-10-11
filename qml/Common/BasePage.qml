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

// Units
import LunaNext.Common 0.1
import LuneOS.Forms 1.0

/*
 * What every page is built on, after the settings apps' BasePage: the grey
 * gradient behind the content and an italic explanation line under it.
 */
Page {
    id: basePage

    // Shown on the right of the header, where the settings apps have their switch
    property Component headerAction: null
    property alias explanation: footerText.text

    background: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#D8D8D8" }
            GradientStop { position: 1.0; color: "#888" }
        }
    }

    footer: ExplanationText {
        id: footerText

        leftPadding: Units.gu(1)
        rightPadding: Units.gu(1)
        bottomPadding: Units.gu(1)
        font.italic: true
    }
}
