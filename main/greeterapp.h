/***************************************************************************
 * Copyright (c) 2015-2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 * Copyright (c) 2013 Nikita Mikhaylov <nslqqq@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 ***************************************************************************/

#ifndef GREETERAPP_H
#define GREETERAPP_H

#include <panel.h>
#include <dsglobal.h>

#include "greeterproxy.h"

class GreeterPanel : public DS_NAMESPACE::DPanel
{
    Q_OBJECT
    Q_PROPERTY(GreeterProxy* proxy READ proxy NOTIFY proxyChanged)
public:
    explicit GreeterPanel(QObject *parent = nullptr);

    virtual bool load() override;

    virtual bool init() override;

    GreeterProxy *proxy() const;

public slots:
    void show();
    void showUserList();
    void showAuth(bool active);
    void suspend(bool enable);
    void hibernate(bool enable);
    bool visible() const;
signals:
    void visibleChanged(bool visible);

signals:
    void proxyChanged();
    void localeChanged(const QString &locale);

private slots:
    void updateTranslation();
    void onLoginFailed(const QString &user);
    void onLoginSuccessed(const QString &user);
private:
    GreeterProxy *m_proxy = nullptr;
    bool m_visible = false;
};

#endif // GREETERAPP_H
