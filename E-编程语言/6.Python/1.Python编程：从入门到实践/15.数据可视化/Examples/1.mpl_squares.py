# coding=utf-8
import matplotlib.pyplot as plt

input_values = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
squares = [1, 4, 9, 16, 25, 36, 49, 64, 81, 100]

plt.plot(input_values, squares, linewidth = 3)

# 设置标题
plt.title("Square Numbers", fontsize=24)

# 设置坐标轴标签
plt.xlabel("Value", fontsize=14)
plt.ylabel("Square of Value", fontsize=14)

# 设置刻度标记的大小
plt.tick_params(axis = 'both', labelsize = 14)
plt.show()
