// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.deepin.dtk
import org.deepin.ds

ContainmentItem {
    id: root
    implicitWidth: 200
    implicitHeight: 300

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: cover
            Layout.alignment:  Qt.AlignTop | Qt.AlignHCenter
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            color: "gray"
        }

        ListView {
            id: authBar
            Layout.topMargin: 10
            Layout.alignment:  Qt.AlignTop | Qt.AlignHCenter
            Layout.preferredHeight: 30
            Layout.preferredWidth: contentWidth
            orientation: ListView.Horizontal
            model: root.Containment.appletItems
            currentIndex: 0
            delegate: ToolButton {
                checked: ListView.isCurrentItem
                text: model.data.icon
                onClicked: {
                    authBar.currentIndex = model.index
                }
            }
        }

        Item {
            Layout.topMargin: 10
            Layout.alignment:  Qt.AlignTop | Qt.AlignHCenter
            Layout.preferredHeight: userIcon.height + userName.height + 10
            Layout.preferredWidth: 100
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
                    top: userIcon.bottom
                    topMargin: 5
                    horizontalCenter: userIcon.horizontalCenter
                }
                text: Panel.proxy.userModel.currentUserName
            }
        }

        StackLayout {
            id: authView
            Layout.alignment:  Qt.AlignBottom | Qt.AlignHCenter
            Layout.preferredHeight: 30
            Layout.fillHeight: false
            Layout.fillWidth: true
            currentIndex: authBar.currentIndex
            Repeater {
                Layout.fillWidth: true
                model: root.Containment.appletItems
                delegate: Loader {
                    id: loader
                    active: StackLayout.isCurrentItem

                    sourceComponent: Control {
                        contentItem: model.data
                    }
                }
            }
        }

        Rectangle {
            Layout.topMargin: 10
            Layout.alignment:  Qt.AlignBottom | Qt.AlignHCenter
            Layout.preferredWidth: 30
            Layout.preferredHeight: 30
            color: "blue"
        }
    }
}
