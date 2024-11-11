// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "appletproxy.h"
#include <applet.h>

#include "lockworker.h"
#include "sessionbasemodel.h"
#include "dbuslockagent.h"
#include "dbusshutdownagent.h"

DS_USE_NAMESPACE

class LockApplet : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(bool Visible READ Visible)
public:
    explicit LockApplet(QObject *parent = nullptr);
    ~LockApplet();

    virtual bool load() override;

    virtual bool init() override;

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

private slots:
    void onFrameChanged();

private:
   QObject *frame() const;

private:
    DAppletProxy *greeterProxy() const;
    LockWorker *m_worker{ nullptr };
    SessionBaseModel *m_sessionModel{ nullptr };
    DBusLockAgent *m_lockAgent{ nullptr };
    DBusShutdownAgent *m_shutdownAgent{ nullptr };
};
