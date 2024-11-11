/***************************************************************************
 * Copyright (c) 2013 Abdurrahman AVCI <abdurrahmanavci@gmail.com>
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

#pragma once

#include <QObject>

class AuthManager;

struct LimitsInfo {
    bool locked;        // 认证锁定状态 --- true: 锁定  false: 解锁
    uint maxTries;      // 最大重试次数
    uint numFailures;   // 失败次数，一直累加
    uint unlockSecs;    // 本次锁定总解锁时间（秒），不会随着时间推移减少
    QString unlockTime; // 解锁时间（本地时间）
};

class AuthInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString prompt READ prompt NOTIFY promptChanged FINAL)
    Q_PROPERTY(AuthStage stage READ stage WRITE setStage NOTIFY stageChanged FINAL)
    Q_PROPERTY(AuthState state READ state WRITE setState NOTIFY stateChanged FINAL)
    Q_DISABLE_COPY(AuthInterface)
public:
    enum AuthStage {
        Create,
        Start,
        Send,
        End,
        Destroyed
    };
    enum AuthType {
        AT_None = 0,                 // none
        AT_Password = 1 << 0,        // 密码
        AT_Fingerprint = 1 << 1,     // 指纹
        AT_Face = 1 << 2,            // 人脸
        AT_ActiveDirectory = 1 << 3, // AD域
        AT_Ukey = 1 << 4,            // ukey
        AT_FingerVein = 1 << 5,      // 指静脉
        AT_Iris = 1 << 6,            // 虹膜
        AT_PIN = 1 << 7,             // PIN
        AT_PAM = 1 << 29,            // PAM
        AT_Custom = 1 << 30,         // 自定义
        AT_All = -1                  // all
    };
    /**
     * @brief The AuthStatus enum
     * 认证状态
     */
    enum AuthState {
        AS_None = -1,   // none
        AS_Success,     // 成功，此次认证的最终结果
        AS_Failure,     // 失败，此次认证的最终结果
        AS_Cancel,      // 取消，当认证没有给出最终结果时，调用 End 会出发 Cancel 信号
        AS_Timeout,     // 超时
        AS_Error,       // 错误
        AS_Verify,      // 验证中
        AS_Exception,   // 设备异常，当前认证会被 End
        AS_Prompt,      // 设备提示
        AS_Started,     // 认证已启动，调用 Start 之后，每种成功开启都会发送此信号
        AS_Ended,       // 认证已结束，调用 End 之后，每种成功关闭的都会发送此信号，当某种认证类型被锁定时，也会触发此信号
        AS_Locked,      // 认证已锁定，当认证类型锁定时，触发此信号。该信号不会给出锁定等待时间信息
        AS_Recover,     // 设备恢复，需要调用 Start 重新开启认证，对应 AS_Exception
        AS_Unlocked     // 认证解锁，对应 AS_Locked
    };
    enum AuthFrameType {
        AF_NONE,
        AF_DDM,
        AF_DA,
        AF_PAM
    };
    explicit AuthInterface(QObject *parent = nullptr);

    void setUser(const QString &user);

    QString prompt() const;
    void setPrompt(const QString &newPrompt);

    virtual bool init() { return true; }

    AuthState state() const;
    void setState(AuthState newState);

    AuthStage stage() const;
    void setStage(AuthStage newStage);

public slots:
    virtual void create() {}
    virtual void start() {}
    virtual void send(const QString &token) {}
    virtual void end() {}
    virtual void destroy() {}

signals:
    void promptChanged();
    void stageChanged();
    void stateChanged();

protected:
    QString m_user;
    AuthStage m_stage;
    AuthState m_state;
    QString m_prompt;
    AuthType m_type{ AT_None };
    LimitsInfo m_limits;
    friend class AuthManager;
};
