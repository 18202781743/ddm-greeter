// Copyright (C) 2023 justforlxz <justforlxz@gmail.com>.
// SPDX-License-Identifier: Apache-2.0 OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import org.deepin.ds
import org.deepin.greeter

Column {
    id: root

    TimeDateWidget {
        id: timedate
        currentLocale :{
            let userModel = Panel.proxy.userModel
            let currentUser = userModel.get(userModel.currentUserName)
            return currentUser.locale
        }
        width: 400
        height: 157
        background: RoundBlur {
            radius: 8
        }
    }
}

