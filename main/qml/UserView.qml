// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.deepin.dtk
import org.deepin.ds

Item {
    id: root
    implicitHeight: userIcon.height + userName.height + 10
    implicitWidth: userIcon.width
    DciIcon {
        id: userIcon
        width: 80
        height: 80
        sourceSize.width: 80
        sourceSize.height: 80
        name: {
            let currentUserName = Panel.proxy.userModel.currentUserName
            let user = Panel.proxy.userModel.get(currentUserName)
            return user ? user.icon : "uos-id"
        }
    }
    Text {
        id: userName
        anchors {
            bottom: parent.bottom
            bottomMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
        text: Panel.proxy.userModel.currentUserName
    }
}
