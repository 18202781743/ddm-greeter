// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Effects
import org.deepin.dtk 1.0 as D
import org.deepin.ds
import org.deepin.greeter

RowLayout {
    id: bottomGroup
    property var buttonSize: 30
    spacing: 15

    Item {
        implicitWidth: bottomGroup.buttonSize + 6
        implicitHeight: bottomGroup.buttonSize + 6
        Layout.alignment: Qt.AlignHCenter
        visible: userList.count > 1

        D.RoundButton {
            id: usersBtn

            property bool expand: false
            icon.name: "login_user"
            icon.width: 16
            icon.height: 16
            width: expand ? bottomGroup.buttonSize + 6 : bottomGroup.buttonSize
            height: expand ? bottomGroup.buttonSize + 6 : bottomGroup.buttonSize
            anchors.centerIn: parent
            hoverEnabled: parent.visible

            D.ToolTip.visible: hovered
            D.ToolTip.text: qsTr("Other Users")

            UserList {
                id: userList
                x: (usersBtn.width - userList.width) / 2 - 10
                y: -userList.height - 10
                onClosed: usersBtn.expand = false
            }

            onClicked: {
                usersBtn.expand = true
                userList.open()
            }

            background: RoundBlur {
                radius: usersBtn.width / 2
            }
        }
    }

    Item {
        implicitWidth: controlView.width
        implicitHeight: controlView.height
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        ListView {
            id: controlView
            height: 36
            width: contentWidth
            orientation: ListView.Horizontal
            spacing: 15
            property var controlApplet: DS.applet("org.deepin.ds.greeter.control")
            model: controlApplet ? controlApplet.appletItems : null
            delegate: Control {
                id: controlItem
                width: 36
                height: 36
                contentItem: model.data
                background: RoundBlur {
                    radius: controlItem.width / 2
                }
            }
        }
    }
}
