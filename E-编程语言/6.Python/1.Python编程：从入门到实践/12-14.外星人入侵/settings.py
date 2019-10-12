class Settings:
    """存储所有配置的类"""

    def __init__(self):
        """初始化游戏的设置"""
        # 屏幕设置
        self.screen_width = 960
        self.screen_height = 600
        self.bg_color = (200, 200, 200)

        # 子弹
        self.bullet_speed_factor = 1
        self.bullet_width = 300
        self.bullet_height = 15
        self.bullet_color = (60, 60, 60)
        # 限制子弹数量
        self.bullet_allowed = 10

        # 外星人
        self.alien_speed_factor = 0.5
        self.fleet_drop_speed = 10
        # 1:右移, -1 左移
        self.fleet_direction = 1

        # 飞船
        self.ship_speed_factor = 0.5
        self.ship_limit = 2     # 3条命

