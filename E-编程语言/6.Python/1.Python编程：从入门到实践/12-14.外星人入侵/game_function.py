import sys
import pygame


def check_events():
    """响应鼠标事件"""
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            print("exit...")
            sys.exit()
