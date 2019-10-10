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
        gf.check_events(ship)
        ship.update()
        gf.update_screen(ai_setting, screen, ship)

run_game()
