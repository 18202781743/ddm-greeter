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

    SessionList {
        id: sessionList
        x: (sessionsBtn.width - sessionList.width) / 2 - 10
        y: -sessionList.height - 10
        onClosed: sessionsBtn.expand = false
    }

    ActionButton {
        id: sessionsBtn

        property bool expand: false
        icon.name: "login_keyboard"
        icon.width: 16
        icon.height: 16
        anchors.centerIn: parent
        hoverEnabled: parent.visible

        ToolTip.visible: hovered
        ToolTip.text: qsTr("Other Sessions")
        onClicked: {
            sessionsBtn.expand = true
            sessionList.open()
        }
    }
}
