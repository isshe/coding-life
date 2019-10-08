import sys

import pygame

from settings import Settings
from ship import Ship
import game_function as gf

def run_game():
    pygame.init()
    ai_settings = Settings()

    screen_size = (ai_settings.screen_width, ai_settings.screen_height)
    screen = pygame.display.set_mode(screen_size)
    pygame.display.set_caption("Alien Invasion")

    ship = Ship(screen)

    # 开始游戏主循环
    while True:
        # 监听键盘和鼠标事件
        gf.check_events()

        # 设置背景颜色
        screen.fill(ai_settings.bg_color)

        # 画飞船
        ship.blit_me()

        # 让最近绘制的屏幕可见
        pygame.display.flip()


run_game()
