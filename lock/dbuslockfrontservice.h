// SPDX-FileCopyrightText: 2011 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSLOCKFRONTSERVICE_H
#define DBUSLOCKFRONTSERVICE_H

#include "lockapplet.h"

#include <QDBusAbstractAdaptor>
#include <QtCore/QObject>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.deepin.dde.LockFront1
 */
class DBusLockFrontService: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.LockFront1")

public:
    explicit DBusLockFrontService(LockApplet *parent);
    virtual ~DBusLockFrontService();

    inline LockApplet *parent() const
    { return static_cast<LockApplet *>(QObject::parent()); }

public:// PROPERTIES
    Q_PROPERTY(bool Visible READ Visible)

    bool Visible();
public Q_SLOTS:// METHODS
    void Show();
    void ShowUserList();
    void ShowAuth(bool active);
    void Suspend(bool enable);
    void Hibernate(bool enable);

Q_SIGNALS:
    void ChangKey(QString key);
    void Visible(bool visible);
};

#endif // DBUSLOCKFRONTSERVICE_H
