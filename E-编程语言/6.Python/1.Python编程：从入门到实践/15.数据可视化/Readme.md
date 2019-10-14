
[TOC]

# 进度
15.4章

# matplotlib使用
```python
import matplotlib.pyplot as plt
```

## 绘制折线图
* plot()
```python
squares = [1, 4, 9, 16, 25]
plt.plot(squares)
plt.plot(squares, linewidth=5)

input_value = [1, 2, 3, 4, 5]   # x轴
plt.plot(input_value, squares, linewidth=5)
```

## 显示图形
* show()
```python
plt.show()
```

## 设置标题
* title()
```python
plt.title("Square Numbers", fontsize=24)
```

## 设置横纵标签
* xlabel()
* ylabel()
```python
plt.xlable("Value", fontsize=14)
plt.ylable("Square of Value", fontsize=14)
```

## 设置刻度标记的大小
* tick_params()
```python
plt.tick_params(axis='both', labelsize=14)
```

## 绘制散点图
* scatter()
```python
# 绘制一个点
plt.scatter(2, 4)

# 绘制多个点
x_value = [1, 2, 3, 4, 5]
y_value = [1, 4, 9, 16, 25]
plt.scatter(x_value, y_value, s=100)

# 自动计算数值
x_value = list(range(1, 1001))
y_value = [x**2 for x in x_value]
plt.scatter(x_value, y_value, s = 40)
```

## 设置坐标轴的取值范围
* axis()
```python
plt.axis([0, 1100, 0, 1100000])
```

## 自动保存图表
* savefig()
```python
# bbox_inches='tight': 将空白区域裁减掉
plt.savefig('abc.png', bbox_inches='tight')
```

## 隐藏坐标轴
```python
plt.axes().get_xaxis().set_visible(False)
plt.axes().get_yaxis().set_visible(False)
```

## 设置图表的宽度、高度、分辨率、背景色
* figure()
```python
# dpi: 分辨率
# figsize: 绘图窗口尺寸，单位英寸
plt.figure(dpi=128, figsize=(10, 6))
```

# Pygal
> 图表类型画廊: http://www.pygal.org/

