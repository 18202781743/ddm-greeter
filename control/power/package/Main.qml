// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.deepin.dtk
import org.deepin.ds

AppletItem {
    id: root
    implicitWidth: 36
    implicitHeight: 36

    PowerList {
        id: powerList
        y: -powerList.height - 10
        x: (powerBtn.width - powerList.width) / 2 - 10
        onClosed: powerBtn.expand = false
    }

    ActionButton {
        id: powerBtn

        property bool expand: false
        icon.name: "login_power"
        icon.width: 16
        icon.height: 16
        anchors.centerIn: parent
        ToolTip.visible: hovered
        ToolTip.text: qsTr("Power")
        onClicked: {
            powerBtn.expand = true
            powerList.open()
        }
    }
}
