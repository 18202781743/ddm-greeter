// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <applet.h>

#include "powerinterface.h"

DS_USE_NAMESPACE

class PowerApplet : public DApplet
{
    Q_OBJECT
    Q_PROPERTY(PowerInterface* proxy READ proxy NOTIFY proxyChanged FINAL)
public:
    explicit PowerApplet(QObject *parent = nullptr);
    ~PowerApplet();

    virtual bool init() override;

    PowerInterface *proxy() const;

signals:
    void proxyChanged();
private:
    PowerInterface *m_proxy{ nullptr };
};
