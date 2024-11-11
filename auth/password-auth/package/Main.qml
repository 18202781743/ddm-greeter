// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.deepin.ds

AppletItem {
    id: root
    property string icon: "password"
    implicitWidth: 60
    implicitHeight: 30

    TextField {
        id: passwordField
        width: parent.width
        height: 30

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        echoMode: false ? TextInput.Password : TextInput.Normal
        maximumLength: 510
        placeholderText: qsTr("Please enter password")

        Keys.onPressed: function (event) {
            if (event.key === Qt.Key_CapsLock) {
                event.accepted = true
            } else if (event.key === Qt.Key_Return) {
                Applet.auth.send(passwordField.text)
                event.accepted = true
            }
        }
    }
}
