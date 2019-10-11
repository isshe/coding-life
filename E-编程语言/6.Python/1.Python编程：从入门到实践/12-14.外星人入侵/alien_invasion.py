import sys

import pygame
from pygame.sprite import Group

from settings import Settings
from ship import Ship
import game_function as gf


def run_game():
    pygame.init()
    ai_settings = Settings()

    screen_size = (ai_settings.screen_width, ai_settings.screen_height)
    screen = pygame.display.set_mode(screen_size)
    pygame.display.set_caption("Alien Invasion")

    ship = Ship(ai_settings, screen)
    bullets = Group()

    # 开始游戏主循环
    while True:
        # 监听键盘和鼠标事件
        gf.check_events(ai_settings, screen, ship, bullets)
        ship.update()
        bullets.update()

        for bullet in bullets.copy():
            if bullet.rect.bottom <= 0:
                bullets.remove(bullet)
        #print(len(bullets))

        gf.update_screen(ai_settings, screen, ship, bullets)


run_game()
