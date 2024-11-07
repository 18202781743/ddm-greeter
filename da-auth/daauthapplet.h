// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "greeterauthinterface.h"

#include <applet.h>

DS_USE_NAMESPACE
DS_BEGIN_NAMESPACE
class DAppletProxy;
DS_END_NAMESPACE

class DAAuth;
class DAAuthApplet : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(GreeterAuthInterface* auth READ auth CONSTANT FINAL)
public:
    explicit DAAuthApplet(QObject *parent = nullptr);
    ~DAAuthApplet();

    GreeterAuthInterface *auth();

    virtual bool load() override;

    virtual bool init() override;

private slots:
    void requestAuthentication(bool enable);

private:
    DAAuth *m_auth = nullptr;
    DAppletProxy *greeterProxy() const;
};
