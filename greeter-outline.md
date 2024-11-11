电源管理
账户管理

登陆锁屏
org.deepin.dde.greeter
ddm-greeter插件 (Containment) org.deepin.dde.greeter
    - 界面切换
    - 插件加载控制

    锁屏主界面 （Panel） (org.deepin.dde.greeter.main)
        网络插件 （AppletItem, PanelPopup）(org.deepin.dde.greeter.network)
        日期插件 (org.deepin.dde.greeter.date)
        用户列表
        会话列表
        电源
        键盘布局
        屏幕键盘
        壁纸
        通知插件（AppletItem）(org.deepin.dde.greeter.notification)
    选择界面
    锁屏扩展界面（认证扩展）
    更新界面

应用状态
    登录、锁屏、桌面
        登录（壁纸、主界面）
        锁屏（壁纸、主界面）
        桌面（壁纸、选项）
            选择选项用户列表 -> 锁屏

窗口层级
    壁纸
    主界面、选项
        主界面（用户、认证、托盘插件（网络）、行为插件（日期））
        选项（电源管理、用户管理）
    会话列表、用户列表、电源列表、网络列表、屏幕键盘

GreeterProxy 单例
    - 初始化各模块
    - 导出模块到qml
    - 流程交互
    - 当前模式（锁屏、桌面）
    - 当前回话
    - 当前用户
    - 当前屏幕
    - 当前认证
    电源管理
        - 电源功能
        - 电源界面切换
    屏幕管理
        - 主屏切换（主界面切换）
        - 屏幕壁纸
    会话管理
        - 会话列表
        - 会话界面
        - 切换会话
    用户管理
        - 用户信息同步
        - 用户界面显示
        - 切换用户信号

    认证管理（dev）认证工厂
        - 根据编译条件，编译支持认证方式
        - 提供认证实现
        - 基础控制流程
        - 支持的认证类型
        - 认证流程
        - 认证状态
        - 认证任务队列（支持切换单因认证、多因认证）
        DA认证（dev）(auth)
        DDM认证（dev）（damauth）
        PAM认证（dev）（pamauth）

认证插件（Containment）(org.deepin.dde.greeter.auth)
    - 根据硬件创建支持认证类型
    - 组合认证界面
    - 当前认证类型
    - 实现认证界面，调用认证过程
    密码认证（AppletItem）(org.deepin.dde.greeter.passwordauth)
    指纹认证（AppletItem）(org.deepin.dde.greeter.signalauth)
    人脸认证（AppletItem）（org.deepin.dde.greeter.faceauth）
    
锁屏扩展插件（Panel）(org.deepin.dde.greeter.one)

treeland中dde-shell插件加载器


认证时机
    create
        userChanged
        visibleChanged
    start
        更新受限
    send
        按键触发
    end
    destroy
    