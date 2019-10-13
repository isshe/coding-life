# coding=utf-8
import matplotlib.pyplot as plt

"""
# 一个点的散点图
plt.scatter(2, 4) #, s = 200)
"""

"""
# 多个点的散点图
x_values = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
y_values = [1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
plt.scatter(x_values, y_values)
"""

# 自动计算数据
x_values = list(range(1, 1001))
y_values = [x**2 for x in x_values]
# plt.scatter(x_values, y_values)
# 删除点轮廓: c(颜色)。没有轮廓，会更细一点
# plt.scatter(x_values, y_values, edgecolor='none')
# plt.scatter(x_values, y_values, c = 'red', edgecolor='none')
plt.scatter(x_values, y_values, c = y_values, cmap=plt.cm.Blues, edgecolor='none') # 颜色渐变

# 设置标题、标签
plt.title("Square Numbers", fontsize = 24)
plt.xlabel("Value", fontsize = 14)
plt.ylabel("Square of Value", fontsize = 14)

# 设置刻度标记的大小
plt.tick_params(axis = "both", which = 'major', labelsize = 14)

plt.show()