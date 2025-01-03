Greeter划分

- greeter插件结构
    - 入口 org.deepin.ds.greeter(Containment)
    - 锁屏主界面 org.deepin.ds.greeter.main(Panel)
        - 快捷插件管理 org.deepin.ds.greeter.quick(ContainmentItem, PanelPopup)
            - 日期插件 org.deepin.ds.greeter.quick.date(AppletItem)
        - 托盘插件管理 org.deepin.ds.greeter.tray(ContainmentItem)
            - 网络插件 org.deepin.ds.greeter.tray.network(AppletItem, PanelPopup)
            - 屏幕键盘插件 org.deepin.ds.greeter.tray.keyboard(AppletItem, PanelPopup)
        - 认证插件管理 org.deepin.ds.greeter.auth(ContainmentItem)
            - 密码认证 org.deepin.ds.greeter.auth.password(AppletItem)
    - 通知插件 org.deepin.ds.greeter.notification(AppletItem)
    - 锁屏扩展界面 org.deepin.ds.greeter.extension(ContainmentItem)
        - 认证扩展 org.deepin.ds.greeter.extension.one(AppletItem)

- greeter界面
    - 锁屏主界面(3)
        - 快捷插件管理界面(1)
            - 日期插件界面(1)
        - 认证插件管理界面(1)
            - 单因子认证界面(2)
            - 多因子认证界面(4)
        - 托盘插件管理界面(1)
            - 屏幕键盘(2)
            - 键盘布局(1)
            - 用户列表(1)
        - 用户列表(1)
        - 会话列表(1)
        - 电源界面(1)
    - 壁纸(1)
    - 关机界面(1)
        - 关机功能(2)
        - 当前选中功能(1)
        - 键盘切换(1)
        - 系统监视器(1)
    - 更新界面(2)
    - 关机阻塞界面(2)
    - 密码认证界面(2)
        - 密码输入
        - 更新提示
        - 修改密码
    - 人脸认证界面(2)
    - 网络插件(10)
    - 锁屏扩展界面
        - 认证扩展(10)
        
- greeter界面状态
    - 登录(壁纸、主界面)
    - 锁屏(壁纸、主界面)
    - 桌面(壁纸、右键选项)
        - 右键选项 -> 锁屏
        - 注销 -> 锁屏 -> 登录

- greeter窗口层级
    - background
        - 壁纸
    - Normal
        - 主界面
            - 用户、认证、托盘插件(网络)、行为插件(日期))
            - 弹框(电源管理、用户管理)
        - 更新界面
        - 关机阻塞界面
    - Top(弹出窗口)
        - 屏幕键盘
        - 通知

GreeterProxy 单例
    - 初始化各模块
    - 导出模块到qml
    - 流程交互
    - 当前模式(锁屏、桌面)
    - 当前回话
    - 当前用户
    - 当前屏幕
    - 当前认证
    电源管理
        - 电源功能
        - 电源界面切换
    屏幕管理
        - 主屏切换(主界面切换)
        - 屏幕壁纸
    Session管理
        - 会话列表
        - 会话界面
        - 切换会话
    用户管理
        - 用户信息同步
        - 用户界面显示
        - 切换用户信号

    认证管理(dev)认证工厂
        - 根据编译条件，编译支持认证方式
        - 提供认证实现
        - 基础控制流程
        - 支持的认证类型
        - 认证流程
        - 认证状态
        - 认证任务队列(支持切换单因认证、多因认证)
        DA认证(dev)(auth)
        DDM认证(dev)(damauth)
        PAM认证(dev)(pamauth)

认证插件(Containment)(org.deepin.ds.greeter.auth)
    - 根据硬件创建支持认证类型
    - 组合认证界面
    - 当前认证类型
    - 实现认证界面，调用认证过程
    密码认证(AppletItem)(org.deepin.ds.greeter.auth.password)
    指纹认证(AppletItem)(org.deepin.ds.greeter.auth.signal)
    人脸认证(AppletItem)(org.deepin.ds.greeter.auth.face)
    
锁屏扩展插件(Panel)(org.deepin.ds.greeter.extension.one)

treeland中dde-shell插件加载器

- 认证插件
    - 单因子认证 org.deepin.ds.greeter.auth.mfacontroller
    - 多因子认证 org.deepin.ds.greeter.auth.sfacontroller
    - 密码认证 org.deepin.ds.greeter.auth.password
    - 人脸认证 org.deepin.ds.greeter.auth.face

- 认证管理 lib
    - AuthManager
        - user 当前用户
        - session 当前会话
        - auths 当前认证实例集合
            - 创建认证实例 createAuth()
            - 删除认证实例 removeAuth()
            - 获取认证实例列表 authLists()
        - backend 认证实现

    - AuthBackend
        - DDMAuth
        - DAAuth
        - PAMAuth
    
    - AuthInterface
        - type 认证类型
        - stage 认证阶段
            - 创建 create()
            - 开始认证 start()
            - 发送认证信息 send()
            - 结束认证 end()
            - 销毁认证 destroy()
        - state 认证状态
        - prompt 提示信息

    - AuthController
        - MFAAuthController 多因子认证
        - SFAAuthController 单因子认证
