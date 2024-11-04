// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import org.deepin.dtk
import org.deepin.ds
import org.deepin.greeter

Popup {
    id: root

    property int count: list.count

    modal: true
    width: 220
    height: 140
    background: RoundBlur {
        radius: 12
    }

    ListView {
        id: list
        spacing: 1
        width: parent.width - 20
        height: parent.height - 20
        x: 10
        y: 10
        clip: true
        model: Panel.proxy.sessionModel
        currentIndex: Panel.proxy.sessionModel.lastIndex
        delegate: ItemDelegate {
            width: list.width
            height: 60
            checked: model.index === list.currentIndex
            onClicked: function () {
                Panel.proxy.sessionModel.lastIndex = index
                root.close()
            }
            text: model.name
            icon.name: model.iconName
        }
    }
}
