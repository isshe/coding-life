# coding=utf-8
import sys

import pygame
from pygame.sprite import Group

from settings import Settings
from ship import Ship
import game_function as gf
from alien import Alien
from game_stats import GameStats
from button import Button


def run_game():
    pygame.init()
    ai_settings = Settings()

    screen_size = (ai_settings.screen_width, ai_settings.screen_height)
    screen = pygame.display.set_mode(screen_size)
    pygame.display.set_caption("Alien Invasion")

    play_button = Button(ai_settings, screen, "Play")
    stats = GameStats(ai_settings)
    ship = Ship(ai_settings, screen)
    bullets = Group()
    aliens = Group()  # Alien(ai_settings, screen)

    # 创建外星人群
    gf.create_fleet(ai_settings, screen, ship, aliens)

    # 开始游戏主循环
    while True:
        # 监听键盘和鼠标事件
        gf.check_events(ai_settings, screen, stats, play_button, ship, aliens, bullets)
        if stats.game_active:
            ship.update()
            gf.update_bullets(ai_settings, screen, ship, aliens, bullets)
            gf.update_aliens(ai_settings, stats, screen, ship, aliens, bullets)

        gf.update_screen(ai_settings, stats, screen, ship, aliens, bullets, play_button)


run_game()
