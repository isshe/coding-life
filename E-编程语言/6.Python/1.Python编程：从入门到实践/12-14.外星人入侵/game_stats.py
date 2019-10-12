class GameStats():
    """跟踪游戏的统计信息"""

    def __init__(self, ai_settings):
        self.ai_settings = ai_settings
        self.game_active = False
        self.ships_left = self.ai_settings.ship_limit
