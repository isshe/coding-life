# coding=utf-8
import sys
import pygame
from bullet import Bullet
from alien import Alien

from time import sleep


def fire_bullet(ai_settings, screen, ship, bullets):
    """如果没有达到数量限制，就创建子弹"""
    if len(bullets) < ai_settings.bullet_allowed:
        new_bullet = Bullet(ai_settings, screen, ship)
        bullets.add(new_bullet)


def check_keydown_event(event, ai_settings, screen, ship, bullets):
    if event.key == pygame.K_RIGHT:
        # 向右移动飞船
        ship.moving_right = True
    elif event.key == pygame.K_LEFT:
        ship.moving_left = True
    elif event.key == pygame.K_q:
        sys.exit()
    else:
        # elif event.key == pygame.K_SPACE:
        fire_bullet(ai_settings, screen, ship, bullets)


def check_keyup_event(event, ship):
    if event.key == pygame.K_RIGHT:
        # 向右移动飞船
        ship.moving_right = False
    elif event.key == pygame.K_LEFT:
        ship.moving_left = False


def check_bullet_alien_collisions(ai_settings, screen, ship, aliens, bullets):
    """相应子弹和外星人的碰撞"""
    # 删除发生碰撞的子弹和外星人
    pygame.sprite.groupcollide(bullets, aliens, True, True)

    if len(aliens) == 0:
        bullets.empty()
        create_fleet(ai_settings, screen, ship, aliens)


def get_number_aliens_x(ai_settings, alien_width):
    available_space_x = ai_settings.screen_width - 2 * alien_width
    number_aliens_x = int(available_space_x / (2 * alien_width))
    return number_aliens_x


def create_alien(ai_settings, screen, aliens, alien_number, row_number):
    alien = Alien(ai_settings, screen)
    alien_width = alien.rect.width
    alien.x = alien_width + 2 * alien_width * alien_number
    alien.rect.x = alien.x
    alien.rect.y = alien.rect.height + 2 * alien.rect.height * row_number
    aliens.add(alien)


def get_number_rows(ai_settings, ship_height, alien_height):
    """计算屏幕可容纳多少行外星人"""
    available_space_y = ai_settings.screen_height - 3 * alien_height - ship_height
    number_rows = int(available_space_y / (2 * alien_height))
    return number_rows


def create_fleet(ai_settings, screen, ship, aliens):
    """创建外星人群"""
    # 创建一个外星人，并计算一行可容纳多少个外星人
    # 外星人间距为一个外星人

    alien = Alien(ai_settings, screen)
    alien_width = alien.rect.width
    number_aliens_x = get_number_aliens_x(ai_settings, alien_width)
    number_rows = get_number_rows(ai_settings, ship.rect.height, alien.rect.height)
    # 创建第一行外星人
    for row_number in range(number_rows):
        for alien_number in range(number_aliens_x):
            # 创建一个外星人并将其加入当前行
            create_alien(ai_settings, screen, aliens, alien_number, row_number)


def change_fleet_direction(ai_settings, aliens):
    """将整群外星人下移，病改变他们的方向"""
    for alien in aliens.sprites():
        alien.rect.y += ai_settings.fleet_drop_speed
    ai_settings.fleet_direction *= -1


def check_fleet_edges(ai_settings, aliens):
    """有外星人到达边缘时，采取相应的措施"""
    for alien in aliens.sprites():
        if alien.check_edges():
            change_fleet_direction(ai_settings, aliens)
            break


def ship_hit(ai_settings, stats, screen, ship, aliens, bullets):
    """响应被外星人撞到的飞船"""
    # 将ships_left减1
    if stats.ships_left > 0:
        stats.ships_left -= 1

        # 情况外星人和子弹列表
        aliens.empty()
        bullets.empty()

        # 创建一群新的外星人，并将飞船放中间
        create_fleet(ai_settings, screen, ship, aliens)
        ship.center_ship()

        sleep(0.5)
    else:
        stats.game_active = False


def check_aliens_bottom(ai_settings, stats, screen, ship, aliens, bullets):
    """检查是否有外星人到达屏幕底端"""
    screen_rect = screen.get_rect()
    for alien in aliens.sprites():
        if alien.rect.bottom >= screen_rect.bottom:
            ship_hit(ai_settings, stats, screen, ship, aliens, bullets)
            break


def check_play_button(ai_settings, screen, stats, play_button, ship, aliens, bullets, mouse_x, mouse_y):
    """检查是否按了开始游戏按钮"""
    if play_button.rect.collidepoint(mouse_x, mouse_y):
        # 重置统计信息
        stats.reset_stats()
        stats.game_active = True

        # 清空外星人和子弹列表
        aliens.empty()
        bullets.empty()

        # 创建外星人，让飞船居中
        create_fleet(ai_settings, screen, ship, aliens)
        ship.center_ship()



def check_events(ai_settings, screen, stats, play_button, ship, aliens, bullets):
    """响应鼠标事件"""
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            print("exit...")
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            check_keydown_event(event, ai_settings, screen, ship, bullets)
        elif event.type == pygame.KEYUP:
            check_keyup_event(event, ship)
        elif event.type == pygame.MOUSEBUTTONDOWN:
            mouse_x, mouse_y = pygame.mouse.get_pos()
            check_play_button(ai_settings, screen, stats, play_button, ship, aliens, bullets, mouse_x, mouse_y)


def update_screen(ai_settings, stats, screen, ship, aliens, bullets, play_button):
    """更新屏幕的图像，并切换到新屏幕"""
    # 设置背景颜色
    screen.fill(ai_settings.bg_color)

    for bullet in bullets.sprites():
        bullet.draw_bullet()

    # 画飞船
    ship.blit_me()
    aliens.draw(screen)

    if not stats.game_active:
        print("draw_button....")
        play_button.draw_button()

    # 让最近绘制的屏幕可见
    pygame.display.flip()


def update_bullets(ai_settings, screen, ship, aliens, bullets):
    bullets.update()

    for bullet in bullets.copy():
        if bullet.rect.bottom <= 0:
            bullets.remove(bullet)

    print(len(bullets), len(aliens))
    check_bullet_alien_collisions(ai_settings, screen, ship, aliens, bullets)


def update_aliens(ai_settings, stats, screen, ship, aliens, bullets):
    """检查是否有外星人位于屏幕边缘，并更新整群外星人的位置"""
    check_fleet_edges(ai_settings, aliens)
    aliens.update()

    # 检测外星人和飞船碰撞
    if pygame.sprite.spritecollideany(ship, aliens):
        ship_hit(ai_settings, stats, screen, ship, aliens, bullets)

    check_aliens_bottom(ai_settings, stats, screen, ship, aliens, bullets)
