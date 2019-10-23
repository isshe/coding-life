[TOC]

Python
---

# 变量
* 规则和指南：
  * 变量名只能包含字母、数字和下划线，但不能以数字开头。
  * 变量名不能包含空格。
  * 不要将python关键字和函数名用作变量名。
  * 变量名应既简短又具描述性。
  * 慎用小写字母`l`和大写字母`O`。和0/1太像了。

# 字符串
## 大小写转换
* str.title(): 每个单词首字母大写。
* str.lower(): 小写。
* str.upper(): 大写。

## 字符串拼接
`+`

## 去除开头/结尾多余空白
* str.rstrip(): 去除结尾的空白。
* str.lstrip(): 去除开头的空白。
* str.strip(): 去除开头和结尾的空白字符。

# 类型转换
* str(): 转换为字符串。
    * str(123)
    
# Python之禅
```
Beautiful is better than ugly.
Explicit is better than implicit.
Simple is better than complex.
Complex is better than complicated.
Flat is better than nested.
Sparse is better than dense.
Readability counts.
Special cases aren't special enough to break the rules.
Although practicality beats purity.
Errors should never pass silently.
Unless explicitly silenced.
In the face of ambiguity, refuse the temptation to guess.
There should be one-- and preferably only one --obvious way to do it.
Although that way may not be obvious at first unless you're Dutch.
Now is better than never.
Although never is often better than *right* now.
If the implementation is hard to explain, it's a bad idea.
If the implementation is easy to explain, it may be a good idea.
Namespaces are one honking great idea -- let's do more of those!
```

# 列表
## 修改元素
```
list = ["isshe", "chudai"]
list[0] = "chudai"
```

## 插入元素
```
list = ["isshe", "chudai"]
list.insert(0, "chudai")
```

## 添加元素
```
list = ["isshe", "chudai"]
list.append("chudai")
```

## 删除元素
* del
```
list = ["isshe", "chudai"]
del list[0]
```

* pop()
```
list = ["isshe", "chudai"]
list.pop()
list.pop(0)
```

* reomve(): 根据值删除
```
list = ["isshe", "chudai"]
list.remove("chudai")
```

# 用户输入
* python 3: `input()`
* python 2.7: `raw_input()`


# 函数
## 编写函数指南
* 应给函数指定描述性名称
* 每个函数应包含简要的阐述其功能的注释
* 函数间可以使用两个空行将其隔开

# 类
> 面向对象编程是最有效的软件编写方法之一。

## 创建类
```python
class ClassName(ParentClassName):
    """一些注释"""
    def __init__(self, args):
        # python 2.7
        # super(ClassName, self).__init__(args)
        args = args
```
* 类中的函数称为`方法`。
* `Python 3`和`Python 2.7`不同，具体见注释。

> 解决问题时，你从较高的逻辑层面（而不是语法层 面）考虑；你考虑
> 不是Python，而是如何使用代码来表示实物。

# 导入
```python
# 导入模块
import module_name
import module_name as new_module_name
# 导入函数
from module_name import function_name
from module_name import function_name as new_function_name
# 导入类
from module_name import ClassName
from module_name import ClassName as NewClassName
from module_name import Class1, Class2

# 导入所有类(不推荐使用)
# 1.这样导入不清楚导入了什么东西
# 2.容易导致冲突
from module_name import *
```

# 解析csv文件
```python
import csv

filename = "xxx.csv"
with open(filename) as f:
    reader = csv.reader(f)
    header_row = next(reader)   # 第一行
    for row in reader:
        pass
```

# 画折线图
```python
from matplotlib import pyplot as plt

x_values = []
y_values = []
fig = plt.figure(dpi=64, figsize=(10, 6))
plt.plot(x_values, y_values, c='blue')
plt.title("Title", fontsize=24)
plt.xlabel('XValue', fontsize=14)
# plt.autofmt_xdate()   # 是日期的时候
plt.ylabel('YValue', fontsize=14)
plt.tick_params(axis='both', which='major', labelsize=16)

plt.show()
```

# 读写json文件
```python
filename = 'xxx.json'
with open(filename) as f:
    json_data = json.load(f)
```

# 常用操作
## 遍历目录
```python
import os
files = os.listdir(path)
for file in files:
    pass
```

## 判断文件类型
```python
import os
os.path.isdir()
os.path.isfile()
os.path.islink()
```

## 按行读取文件
```python
with open(filename, mode) as f:
    for line in f.readlines():
        pass
```

## 多线程
```python
from threading import Thread

def thread_func(a, b, c):
    pass

t = Thread(target=thread_func, args=(a, b, c))
t.start()

```

## 命令行参数解析
```python
parser = optparse.OptionParser("Usage%prog " + "-f <zipfile> -d <dictionary>")
parser.add_option('-f', dest='zname', type='string', help='specify zip file')
parser.add_option('-d', dest='dname', type='string', help='specify dictionary file')

(options, args) = parser.parse_args()
if options.zname == None | options.dname == None:
    print(parser.usage)
    exit(0)
else:
    zname = options.zname
    dname = options.dname
```


