// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.deepin.ds
import org.deepin.greeter

FocusScope {
    id: root
    clip: true

    signal animationPlayed
    signal animationPlayFinished

    // WallpaperController {
    //     id: wallpaperController
    //     output: root.output
    //     lock: true
    //     type: WallpaperController.Normal
    // }

    // prevent event passing through greeter
    MouseArea {
        anchors.fill: parent
        enabled: true
    }

    property int state: LoginAnimation.Show

    Loader {
        id: leftAnimation
        anchors.fill: parent
        sourceComponent: LoginAnimation {
            anchors.fill: parent
            target: quickAction
            state: root.state
        }
    }

    Loader {
        id: logoAnimation
        anchors.fill: parent
        sourceComponent: LoginAnimation {
            anchors.fill: parent
            target: logo
            state: root.state
        }
    }

    Loader {
        id: rightAnimation
        anchors.fill: parent
        sourceComponent: LoginAnimation {
            state: root.state
            target: userInput
            anchors.fill: parent
            onStopped: {
                if (state === LoginAnimation.Hide) {
                    animationPlayFinished()
                }
            }
        }
    }

    Loader {
        id: bottomAnimation
        anchors.fill: parent
        sourceComponent: LoginAnimation {
            state: root.state
            target: controlAction
            anchors.fill: parent
        }
    }

    Item {
        id: leftComp
        z: -1
        width: parent.width * 0.4
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            leftMargin: 30
            topMargin: 30
            bottomMargin: 30
        }
    }

    Item {
        id: rightComp
        z: -1
        anchors {
            left: leftComp.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            leftMargin: 30
            rightMargin: 30
            topMargin: 30
            bottomMargin: 30
        }
    }

    QuickAction {
        id: quickAction
        anchors {
            verticalCenter : parent.verticalCenter
            right: leftComp.right
            rightMargin: leftComp.width / 9
        }
    }

    Row {
        id: logo
        anchors {
            bottom: leftComp.bottom
            left: leftComp.left
        }

        Image {
            id: logoPic
            source: logoProvider.logo
            width: 120
            height: 120
            fillMode: Image.PreserveAspectFit
        }

        Text {
            text: logoProvider.version
            font.weight: Font.Normal
            font.pixelSize: logoPic.height / 2
            color: Qt.rgba(1, 1, 1, 153 / 255)
        }
    }

    UserInput {
        id: userInput
        anchors {
            verticalCenter: parent.verticalCenter
            left: rightComp.left
            leftMargin: rightComp.width / 5
        }

        focus: true
    }

    ControlAction {
        id: controlAction
        anchors {
            bottom: rightComp.bottom
            right: rightComp.right
        }
    }

    // Delay start animation, Because when the Qt control plays animation during initialization, the effect cannot be seen.
    Timer {
        id: delayStart
        interval: 100
        running: true
        repeat: false
        onTriggered: {
            // wallpaperController.type = WallpaperController.Scale
            leftAnimation.item.start({x: root.x - quickAction.width, y: quickAction.y}, {x: quickAction.x, y: quickAction.y})
            logoAnimation.item.start({x: root.x - logo.width, y: logo.y}, {x: logo.x, y: logo.y})
            rightAnimation.item.start({x: root.width + userInput.width, y: userInput.y}, {x: userInput.x, y: userInput.y})
            bottomAnimation.item.start({x: controlAction.x, y: controlAction.y + controlAction.height}, {x: controlAction.x, y: controlAction.y})
        }
    }

    Connections {
        target: Panel.proxy
        function onStateChanged() {
            switch (Panel.proxy.state) {
                case Proxy.AuthSucceeded: {
                    userInput.userAuthSuccessed()
                    userInput.updateHintMsg(userInput.normalHint)
                    Panel.proxy.quit()
                }
                break
                case Proxy.AuthFailed: {
                    userInput.userAuthFailed()
                    userInput.updateHintMsg(qsTr("Password is incorrect."))
                }
                break
                case Proxy.Quit: {
                    // wallpaperController.type = WallpaperController.Normal
                    root.state = LoginAnimation.Hide
                    leftAnimation.item.start({x: quickAction.x, y: quickAction.y}, {x: root.x - quickAction.width, y: quickAction.y})
                    logoAnimation.item.start({x: logo.x, y: logo.y}, {x: root.x - logo.width, y: logo.y})
                    rightAnimation.item.start({x: userInput.x, y: userInput.y}, {x: root.width + userInput.width, y: userInput.y})
                    bottomAnimation.item.start({x: controlAction.x, y: controlAction.y}, {x: controlAction.x, y: controlAction.y + controlAction.height})
                    animationPlayed()
                }
                break
            }
        }

        function onCurrentUserChanged() {
            console.log("update user")
            userInput.updateUser()
        }

        function onLocaleChanged(locale) {
            console.log("translation updated")
            logoProvider.updateLocale(locale)
        }
    }

    LogoProvider {
        id: logoProvider
    }

    // Component.onDestruction: {
    //     wallpaperController.lock = false
    // }
}
