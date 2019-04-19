# CSS

## 1. 什么是CSS
* CSS: 叠层样式表（Cascading style Sheets）
* 样式定义`如何显示`HTML元素；
* 样式通常存储在`样式表`中；
* 把样式添加到HTML4.0中，是为了`解决内容与表现分离的问题`；
* 外部样式表可以极大提高工作效率；
* 外部样式表通常存储在CSS文件中；
* 多个样式定义可叠层为一个

## 2. CSS语法
![CSS规则](./css_rule.jpg)
* CSS规则由两个主要的部分构成：选择器、一条或多条声明。

## 3. CSS 注释
```css
/*这是个注释*/
p
{
text-align:center;
/*这是另一个注释*/
color:black;
font-family:arial;
}
```

## 4. CSS id和class选择器
* id选择器可以给一个特定的HTML元素使用；（以`#`定义）【见：[1_ex](./Examples/1_ex_id_selector.html)】
* class选择器可以再多个元素中使用；（以`.`定义）【见：[2_ex](./Examples/2_ex_class_selector_multi_use.html)】
    * 也可以指定特定的HTML元素使用class。【见：[3_ex](./Examples/3_ex_class_selector_single_use.html)】
* id/class属性不要以数字开头，数字开头的ID在某些浏览器中不起作用。（如：Firefox）

## 5. 创建/插入样式表
插入样式表的方法有三种:
* 外部样式表(External style sheet)
```html
<head>
<link rel="stylesheet" type="text/css" href="mystyle.css">
</head>
```
* 内部样式表(Internal style sheet)
```html
<head>
<style>
hr {color:sienna;}
p {margin-left:20px;}
body {background-image:url("images/back40.gif");}
</style>
</head>
```
* 内联样式(Inline style)
多重样式优先级
* 内联样式）Inline style > （内部样式）Internal style sheet >（外部样式）External style sheet > 浏览器默认样式

## 6. 背景
### 6.1 背景颜色、图片、位置
|属性|描述|
|----|----|
|background|简写属性，作用是将背景属性设置在一个声明中。|
|background-attachment|背景图像是否固定或者随着页面的其余部分滚动。|
|background-color|设置元素的背景颜色。|
|background-image|把图像设置为背景。|
|background-position|设置背景图像的起始位置。|
|background-repeat|设置背景图像是否及如何重复。|

* 属性可简写，简写顺序为：
    * background-color
    * background-image
    * background-repeat
    * background-attachment
    * background-position
* 示例：
```html
body {background-color:#b0c4de;}
body {background-image:url('paper.gif');}
background-repeat:no-repeat;
background-position:right top;
```
简写示例：
```html
body {background:#b0c4de url('paper.gif') no-repeat right top;}
```

## 7，文本
|属性|描述|
|----|----|
|color|设置文本颜色|
|direction|设置文本方向|
|letter-spacing|设置字符间距|
|line-height|设置行高|
|text-align|对齐元素中的文本|
|text-decoration|向文本添加修饰|
|text-indent|缩进元素中文本的首行|
|text-shadow|设置文本阴影|
|text-transform|控制元素中的字母|
|unicode-bidi|设置或返回文本是否被重写|
|vertical-align|设置元素的垂直对齐|
|white-space|设置元素中空白的处理方式|
|word-spacing|设置字间距|

### 7.1 文本颜色
```html
body {color:red;}
h1 {color:#00ff00;}
h2 {color:rgb(255,0,0);}
```
### 7.2 对齐方式
```html
h1 {text-align:center;}
p.date {text-align:right;}
p.main {text-align:justify;}
```

### 7.3 文本修饰
```html
a {text-decoration:none;}
h1 {text-decoration:overline;}
h2 {text-decoration:line-through;}
h3 {text-decoration:underline;}
```

### 7.4 文本转换
```html
p.uppercase {text-transform:uppercase;}
p.lowercase {text-transform:lowercase;}
p.capitalize {text-transform:capitalize;}
```

### 7.5 文本缩进
```html
p {text-indent:50px;}
```

## 8. 字体
|属性|描述|
|----|----|
|font|在一个声明中设置所有的字体属性|
|font-family|指定文本的字体系列|
|font-size|指定文本的字体大小|
|font-style指定文本的字体样式|
|font-variant|以小型大写字体或者正常字体显示文本|
|font-weight|指定字体的粗细|

## 9. 链接
链接状态：
* a:link - 正常，未访问过的链接
* a:visited - 用户已访问过的链接
* a:hover - 当用户鼠标放在链接上时
* a:active - 链接被点击的那一刻
```html
a:link {color:#000000;}      /* 未访问链接*/
a:visited {color:#00FF00;}  /* 已访问链接 */
a:hover {color:#FF00FF;}  /* 鼠标移动到链接上 */
a:active {color:#0000FF;}  /* 鼠标点击时 */
```

## 10. 列表
|属性|描述|
|---|---|
|list-style|简写属性。用于把所有用于列表的属性设置于一个声明中|
|list-style-image|将图象设置为列表项标志|
|list-style-position|设置列表中列表项标志的位置|
\list-style-type|设置列表项标志的类型|

