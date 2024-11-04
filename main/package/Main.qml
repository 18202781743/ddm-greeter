// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.deepin.ds
import org.deepin.greeter

Window {
    id: root
    width: 800
    height: 600
    // width: Screen.width
    // height: Screen.height
    visible: true

    Image {
        anchors.fill: parent
        source: "file:///usr/share/wallpapers/deepin/deepin-default.jpg"
    }

    Rectangle {
        id: cover
        anchors.fill: parent
        color: "transparent"
        // opacity: wallpaperController.type === WallpaperController.Normal ? 0 : 0.9
        Behavior on opacity {
            PropertyAnimation {
                duration: 1000
                easing.type: Easing.OutExpo
            }
        }
    }

    Greeter {
        anchors.fill: parent
    }
}
