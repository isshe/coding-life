import os
import random


def pick_color():
    return "#"+''.join([random.choice('ABCDEF0123456789') for i in range(6)])


def pick_image(path):
    files = os.listdir(path)
    return random.choice(files)
