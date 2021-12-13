import os
import random


def pick_color():
    red = random.randint(80, 230)
    green = random.randint(80, 230)
    blue = random.randint(80, 230)
    return '#%02x%02x%02x' % (red, green, blue)
    # return "#"+''.join([random.choice('ABCDEF0123456789') for i in range(6)])


def pick_image(path):
    files = os.listdir(path)
    return random.choice(files)


if __name__ == "__main__":
    print(pick_color())
