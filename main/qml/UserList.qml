// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import org.deepin.dtk 1.0 as D
import org.deepin.ds
import org.deepin.greeter

D.Popup {
    id: users

    property int count: Panel.proxy.userModel.count
    property int userItemHeight: 44
    property int maxListHeight: userItemHeight * 5 + padding * 2
    property bool useScrollBar: listv.contentHeight > maxListHeight

    focus: true
    padding: 6
    modal: true
    width: 232
    height: useScrollBar ? maxListHeight : listv.contentHeight + padding * 2

    background: RoundBlur {
        radius: 12
    }

    function selectCurrentUser(userName, index) {
        console.log("selected user", userName)
        Panel.proxy.activateUser(userName)
        userList.close()
    }

    onAboutToShow: {
        //FIXME: we shouldn't use index directly
        listv.positionViewAtIndex(listv.currentIndex, ListView.Beginning)
    }

    ListView {
        id: listv
        anchors.fill: parent
        model: Panel.proxy.userModel
        width: parent.width
        focus: true
        clip: true
        keyNavigationWraps: true
        highlightMoveDuration: 0
        Keys.onTabPressed: listv.incrementCurrentIndex()
        Keys.onReturnPressed: selectCurrentUser(model.name, model.index)
        currentIndex: {
            for (let i = 0; i < listv.model.count; i++) {
                let item = listv.model.get(i)
                if (item.name === Panel.proxy.userModel.currentUserName)
                    return i;
            }

            return -1
        }

        property D.Palette highlightColor: D.Palette {
            normal: D.DTK.makeColor(D.Color.Highlight)
        }

        delegate: D.CheckDelegate {
            id: singleUser
            height: 44
            width: 220
            padding: 4
            focus: true

            contentItem: RowLayout {
                Control {
                    Layout.preferredHeight: 36
                    Layout.preferredWidth: 36
                    Layout.alignment: Qt.AlignVCenter
                    background: Item {
                        D.QtIcon {
                            id: source
                            anchors.fill: parent
                            fallbackSource: model.icon
                            visible: false
                        }
                        Rectangle {
                            id: maskSource
                            radius: 2
                            anchors.fill: source
                            visible: false
                        }
                        D.OpacityMask {
                            anchors.fill: source
                            source: source
                            maskSource: maskSource
                            invert: false
                        }
                        Rectangle {
                            radius: 2
                            anchors.fill: source
                            color: "transparent"
                            border {
                                width: 1
                                color: Qt.rgba(0, 0, 0, 0.2)
                            }
                        }
                    }

                    Item {
                        width: 8
                        height: 8
                        anchors {
                            right: parent.right
                            bottom: parent.bottom
                        }
                        visible: model.logined

                        Rectangle {
                            anchors.centerIn: parent
                            color: Qt.rgba(0, 1, 2 / 250, 1)
                            height: 7
                            width: 7
                            radius: 4
                            border {
                                width: 1
                                color: Qt.rgba(1, 1, 1, 0.8)
                            }
                            z: 1

                            D.BoxShadow {
                                anchors.fill: parent
                                shadowBlur: 4
                                shadowColor: Qt.rgba(0, 180 / 255,
                                                     38 / 255, 0.21)
                                shadowOffsetX: 0
                                shadowOffsetY: 2
                                cornerRadius: parent.radius
                                hollow: true
                            }
                        }

                        Rectangle {
                            anchors.centerIn: parent
                            color: "transparent"
                            height: 6
                            width: 6
                            radius: 4
                            border {
                                width: 1
                                color: Qt.rgba(0, 0, 0, 0.05)
                            }
                            z: 2

                            D.BoxShadow {
                                anchors.fill: parent
                                shadowBlur: 3
                                shadowColor: Qt.rgba(0, 0, 0, 0.2)
                                shadowOffsetX: 0
                                shadowOffsetY: 0
                                cornerRadius: parent.radius
                            }
                        }
                    }
                }

                Column {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 36
                    Layout.alignment: Qt.AlignVCenter
                    Text {
                        id: displayUserName
                        text: model.realName.length === 0 ? model.name : model.realName
                        color: singleUser.ListView.isCurrentItem ? "white" : "black"
                        font: D.DTK.fontManager.t9
                    }
                    Text {
                        text: model.identity
                        color: singleUser.ListView.isCurrentItem ? Qt.rgba(1, 1, 1,0.7)
                                                                 : Qt.rgba(0, 0, 0, 0.6)
                        font: D.DTK.fontManager.t10
                    }
                }
            }

            background: Rectangle {
                id: userBackground
                anchors.fill: parent
                radius: 6
                color: singleUser.ListView.isCurrentItem ? listv.D.ColorSelector.highlightColor : "transparent"

                D.BoxShadow {
                    anchors.fill: parent
                    shadowBlur: 0
                    shadowColor: Qt.rgba(17 / 255, 47 / 255, 251 / 255, 0.5)
                    shadowOffsetX: 0
                    shadowOffsetY: -1
                    cornerRadius: parent.radius
                    hollow: true
                }
            }

            onClicked: selectCurrentUser(model.name, index)
            checked: listv.currentIndex === model.index
        }
    }
}
