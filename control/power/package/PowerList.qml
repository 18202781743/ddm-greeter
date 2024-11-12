// Copyright (C) 2023 ComixHe <heyuming@uniontech.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import org.deepin.ds
import org.deepin.greeter
import org.deepin.dtk 1.0 as D

D.Menu {
    id: powers
    modal: true
    padding: 6
    width: 122

    background: RoundBlur {
        radius: 12
    }

    D.Action {
        enabled: Applet.proxy.canHibernate
        text: qsTr("Hibernate")
        icon.name: "login_sleep"

        onTriggered: Applet.proxy.hibernate()
    }

    D.Action {
        enabled: Applet.proxy.canSuspend
        text: qsTr("Suspend")
        icon.name: "login_suspend"

        onTriggered: Applet.proxy.suspend()
    }

    D.Action {
        enabled: Applet.proxy.canReboot
        text: qsTr("Reboot")
        icon.name: "login_reboot"

        onTriggered: Applet.proxy.reboot()
    }

    D.Action {
        id: powerOff
        enabled: Applet.proxy.canPowerOff
        text: qsTr("Shut Down")
        icon.name: "login_poweroff"

        onTriggered: Applet.proxy.powerOff()
    }

    onAboutToShow: {
        currentIndex = count - 1
    }

    delegate: D.MenuItem {
        id: item
        height: enabled ? 26 : 0
        display: D.IconLabel.IconBesideText
        font: D.DTK.fontManager.t9
        visible: enabled

        property D.Palette highlightColor: D.Palette {
            normal: D.DTK.makeColor(D.Color.Highlight)
        }

        Keys.onUpPressed: {
            let nextIndex = powers.currentIndex - 1
            for (var index = 0; index < count; index++) {
                if (nextIndex < 0) {
                    nextIndex += powers.count
                }

                let tmp = powers.itemAt(nextIndex)
                if (tmp.visible) {
                    break
                }
                nextIndex--
            }

            powers.currentIndex = nextIndex
        }

        Keys.onDownPressed: {
            let nextIndex = powers.currentIndex + 1
            for (var index = 0; index < count; index++) {
                if (nextIndex >= powers.count) {
                    nextIndex -= powers.count
                }

                let tmp = powers.itemAt(nextIndex)
                if (tmp.enabled) {
                    break
                }
                nextIndex++
            }

            powers.currentIndex = nextIndex
        }

        background: Rectangle {
            id: backgroundColor
            radius: 6
            anchors.fill: parent
            D.BoxShadow {
                anchors.fill: parent
                shadowBlur: 0
                shadowOffsetX: 0
                shadowOffsetY: -1
                cornerRadius: parent.radius
                hollow: true
            }
            color: powers.itemAt(powers.currentIndex)
                   === item ? item.D.ColorSelector.highlightColor : "transparent"
        }
    }
}
