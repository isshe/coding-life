# coding=utf-8
import pygame


class Ship:
    def __init__(self, ai_settings, screen):
        """初始化飞船并设置初始位置"""
        self.ai_settings = ai_settings
        self.screen = screen

        self.image = pygame.image.load("resources/ship.bmp")
        self.rect = self.image.get_rect()
        self.screen_rect = screen.get_rect()

        # 将每艘飞船放在屏幕底部中央
        self.rect.centerx = self.screen_rect.centerx
        self.rect.bottom = self.screen_rect.bottom

        self.moving_right = False
        self.moving_left = False

        self.center = float(self.rect.centerx)

    def update(self):
        """根据标记移动飞船"""
        if self.moving_right and self.rect.right < self.screen_rect.right:
            self.center += self.ai_settings.ship_speed_factor
        elif self.moving_left and self.rect.left > self.screen_rect.left:
            self.center -= self.ai_settings.ship_speed_factor
        self.rect.centerx = self.center

    def blit_me(self):
        """在指定位置绘制飞船"""
        self.screen.blit(self.image, self.rect)

    def center_ship(self):
        self.center = self.screen_rect.centerx
