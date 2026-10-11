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

// Units
import LunaNext.Common 0.1

import harbour.messwerk.MesswerkWidgets 1.0
import LuneOS.Forms 1.0

/*
 * One measured quantity: its name and current value as a LabelAndValue row,
 * with a scrolling plot of its recent history underneath.
 */
Column {
    id: plotRow

    property alias label: valueRow.label
    property alias value: valueRow.value

    width: parent ? parent.width : implicitWidth

    function addValue(v) {
        plot.addValue(v);
        plot.update();
    }

    function reset() {
        plot.reset();
    }

    LabelAndValue {
        id: valueRow
        width: parent.width
    }

    PlotWidget {
        id: plot

        width: parent.width
        height: Units.gu(12)

        plotColor: "#2071bb"
        scaleColor: "#ADADAD"
        textColor: "#666666"
    }
}
