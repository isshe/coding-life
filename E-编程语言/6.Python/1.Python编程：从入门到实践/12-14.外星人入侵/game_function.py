import sys
import pygame


def check_events(ship):
    """响应鼠标事件"""
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            print("exit...")
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RIGHT:
                # 向右移动飞船
                ship.moving_right = True
            elif event.key == pygame.K_LEFT:
                ship.moving_left = True
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_RIGHT:
                # 向右移动飞船
                ship.moving_right = False
            elif event.key == pygame.K_LEFT:
                ship.moving_left = False


def update_screen(ai_setting, screen, ship):
    """更新屏幕的图像，并切换到新屏幕"""
    # 设置背景颜色
    screen.fill(ai_settings.bg_color)

    # 画飞船
    ship.blit_me()

    # 让最近绘制的屏幕可见
    pygame.display.flip()