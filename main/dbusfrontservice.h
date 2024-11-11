// SPDX-FileCopyrightText: 2011 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSFRONTSERVICE_H
#define DBUSFRONTSERVICE_H

#include "greeterapp.h"

#include <QDBusAbstractAdaptor>
#include <QtDBus>

/*
 * Proxy class for interface org.deepin.dde.Front1
 */
class DBusFrontService: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.LockFront1")

public:
    explicit DBusFrontService(GreeterPanel *parent);
    virtual ~DBusFrontService();

    inline GreeterPanel *parent() const
    { return static_cast<GreeterPanel *>(QObject::parent()); }

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

#endif // DBUSFRONTSERVICE_H
