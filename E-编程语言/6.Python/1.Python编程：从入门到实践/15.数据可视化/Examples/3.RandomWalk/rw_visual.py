# coding=utf-8
import  matplotlib.pyplot as plt

from random_walk import RandomWalk

# 只有程序处于活动状态，就不断地模拟随机漫步
while True:
    # 创建一个RandomWalk实例，并将其包含的点绘制出来
    rw = RandomWalk() #50000)
    rw.fill_walk()

    # 设置绘图窗口尺寸
    plt.figure(dpi=128, figsize=(10, 6))

    point_numbers = list(range(rw.num_points))
    plt.scatter(rw.x_values, rw.y_values, c=point_numbers, cmap=plt.cm.Blues, edgecolor='none')
    # 单独绘制起点和终点
    plt.scatter(0, 0, c='green', edgecolors='none')
    plt.scatter(rw.x_values[-1], rw.y_values[-1], c='red', edgecolors='none')

    # 隐藏坐标轴
    plt.axes().get_xaxis().set_visible(False)
    plt.axes().get_yaxis().set_visible(False)

    plt.show()

    keep_running = input("Make another walk?(y/n): ")
    if keep_running == 'n':
        break