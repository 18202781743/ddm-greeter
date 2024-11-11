// SPDX-FileCopyrightText: 2011 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "dbusfrontservice.h"

/*
 * Implementation of interface class DBusFront
 */

DBusFrontService::DBusFrontService(GreeterPanel *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}

DBusFrontService::~DBusFrontService()
{
}

bool DBusFrontService::Visible()
{
    return parent()->visible();
}

void DBusFrontService::Show()
{
    parent()->show();
}

void DBusFrontService::ShowUserList()
{
    parent()->showUserList();
}

void DBusFrontService::ShowAuth(bool active)
{
    parent()->showAuth(active);
}

void DBusFrontService::Suspend(bool enable)
{
    parent()->suspend(enable);
}

void DBusFrontService::Hibernate(bool enable)
{
    parent()->hibernate(enable);
}
