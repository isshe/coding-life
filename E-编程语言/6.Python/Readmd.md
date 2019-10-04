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



