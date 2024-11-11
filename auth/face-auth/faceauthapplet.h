// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "authinterface.h"

#include <applet.h>
#include <appletproxy.h>

DS_USE_NAMESPACE

class FaceAuthApplet : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(AuthInterface *auth READ auth NOTIFY authChanged FINAL)
public:
    explicit FaceAuthApplet(QObject *parent = nullptr);
    ~FaceAuthApplet();

    AuthInterface *auth();

    virtual bool load() override;

    virtual bool init() override;

signals:
    void authChanged();

private:
    AuthInterface *m_auth = nullptr;
    DAppletProxy *greeterProxy() const;
};
