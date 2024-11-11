// SPDX-FileCopyrightText: 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <containment.h>

DS_USE_NAMESPACE

class AuthApplet : public DContainment
{
    Q_OBJECT
public:
    explicit AuthApplet(QObject *parent = nullptr);
    ~AuthApplet();

    virtual bool load() override;

    virtual bool init() override;

signals:
    void loginFailed(const QString &user);
    void loginSuccessed(const QString &user);

protected:
    QString userName() const;
    bool active() const;

private slots:
    void requestAuthentication(bool active);
    void initAuth();

private:
    QObject *greeter() const;
    QObject *greeterProxy() const;
    QObject *userModel() const;
};
