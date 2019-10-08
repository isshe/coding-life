import pygame


class Ship:
    def __init__(self, screen):
        """初始化飞船并设置初始位置"""
        self.screen = screen

        self.image = pygame.image.load("resources/ship_purple.bmp")
        self.rect = self.image.get_rect()
        self.screen_rect = screen.get_rect()

        # 将每艘飞船放在屏幕底部中央
        self.rect.centerx = self.screen_rect.centerx
        self.rect.bottom = self.screen_rect.bottom

    def blit_me(self):
        """在指定位置绘制飞船"""
        self.screen.blit(self.image, self.rect)
