
# Lua

## 1. 变长参数
### 1.1 遍历
```lua
function xxx(...)
    for k, v in ipairs({...}) do
        -- do something
    end
 end
```

### 1.2 遍历——存在`nil`的情况
```lua
function xxx(...)
    local arg = table.pack(...)
    for i = 1, arg.n do
        -- do something
    end
 end
```

### 1.3 遍历——`select`
```lua
function xxx(...)
    for i = 1, select("#", ...) do
        x = select(i, ...)
        -- do something
    end
 end
```

## 2. table.pack和table.unpack
打包成table或者把table拆了。

## 3. 安全操作
```lua
zip = a and a.b and a.b.c and a.b.c.d
```
可替换为：
```lua
E = {}
zip = (((a or E).b or E).c or E).d
```

## 4.模式匹配
* 魔法字符(magic character)：`( ) . % + - * ? [ ] ^ $`
* 预置字符分类：
    * `.`: 任意字符
    * `%a`: 字母
    * `%c`: 控制字符
    * `%d`: 数字
    * `%g`: 除空格外的可打印字符
    * `%l`: 小写字母
    * `%p`: 标点符号
    * `%s`: 空白字符
    * `%u`: 大写字母
    * `%w`: 字母和数字
    * `%x`: 十六进制数字
    * `%b xy`: 匹配x开头y结尾的字符串
    ```lua
      s = "a (in) line"
      print(string.gsub(s, "%b()", ""))  --> a  line
    ```
    * `%f[char-set]`: 前置模式。【不懂！！！？？？】
* 修饰符：
    * `+`: 重复1次或多次
    * `*`: 重复0次或多次
    * `-`: 重复0次或多次（最小匹配）
    * `?`: 可选（出现0或1次）
* 补集：`^`和`分类的大写`
    * `[^0-7]`, 匹配8进制以外的字符。
    * `[^\n]`, 匹配除换行符以外的字符。
    * `[%S]` = `[^%s]`, 匹配非空白字符。
* string.gsub()
    * 第三个参数可以为：字符串、函数、表。
    * 函数：则取返回值；
    * 表：则把第一个捕获到的内容作为key，到表中找key对应的值。
    
## 5. 时间和日期
* `os.time()`: 以数字形式返回当前的日期和时间。
* `os.date()`: 一定程度上是os.time()的反函数。
    * `os.date("*t", os.time())`: 返回一个表示当前时间的table。
* os.date()的指示符：
    * `%a`: 星期几的缩写，如：Wed
    * `%A`: 星期几的全名，如：Wednesday
    * `%b`: 月份的缩写，如：Sep
    * `%B`: 月份的全名，如：September
    * `%c`: 日期和实践，如：09/16/98 23:48:10
    * `%d`: 一个月中的第几天，取值：`[01-31]`
    * `%H`: 24小时制中的小时数，取值：`[00-23]`
    * `%I`: 12小时制中的小时数，取值: `[01-12]`
    * `%j`: 一年中的第几天，取值：`[001-365]`
    * `%m`: 月份，取值：`[01-12]`
    * `%M`: 分钟，取值：`[00-59]`
    * `%p`: "am"/"pm"
    * `%S`: 秒数，取值：`[00-60]`
    * `%w`: 星期几数字表示，取值：`[0-6=subday-Saturday]`
    * `%W`: 一年中的第几周，取值：`[00-53]`
    * `%x`: 日期，如：09/16/98
    * `%X`: 实践，如：23:48:10
    * `%y`: 两位数的年份，取值：`[00-99]`, 如：98
    * `%Y`: 完整的年份，如：1998
    * `%z`: 时区，如: -0300
    * `%%`: 百分号