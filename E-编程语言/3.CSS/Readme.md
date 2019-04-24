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
|list-style-type|设置列表项标志的类型|

## 11. 表格
### 11.1 边框/折叠边框
* border：设置CSS表格边框
* border-collapse：折叠边框
```css
table, th, td
{
    border: 1px solid black;
}

// 注意，在上面的代码中的表格有双边框。这是因为表和th/td元素有独立的边界；使用下面的代码折叠边框。

table
{
    border-collapse:collapse;
}
```

### 11.2 宽度/高度
* width和height属性定义表格的宽度和高度。
```css
table 
{
    width:100%;
}
th
{
    height:50px;
}
// 设置100％的宽度，50像素的th元素的高度的表格：
```

### 11.3 文字对齐
* text-align：水平对齐；
* vertical-align：垂直对齐；
```css
td
{
    height:50px;
    text-align:right;
    vertical-align:bottom;
}
```

### 11.4 颜色
* background-color：背景颜色
* color：字体颜色
```css
table, td, th
{
    border:1px solid green;     // 边框颜色
}
th
{
    background-color:green;     // th元素的背景颜色
    color:white;                // th元素的文本颜色
}
```

## 12. 盒子模型
**所有HTML元素可以看作盒子**
CSS盒模型本质上是一个盒子，封装周围的HTML元素，它包括：边距，边框，填充，和实际内容。
盒子模型允许我们在其它元素和周围元素边框之间的空间放置元素。
![盒子模型](./box-model.gif)
* Margin(外边距) - 清除边框外的区域，外边距是透明的。
* Border(边框) - 围绕在内边距和内容外的边框。
* Padding(内边距) - 清除内容周围的区域，内边距是透明的。
* Content(内容) - 盒子的内容，显示文本和图像。

### 12.1 元素的宽度和高度
**重要: 当您指定一个CSS元素的宽度和高度属性时，你只是设置内容区域的宽度和高度。要知道，完全大小的元素，你还必须添加填充，边框和边距。**
```css
div {
    width: 300px;
    border: 25px solid green;
    padding: 25px;
    margin: 25px;
}
```

## 13. CSS 边框
|属性|描述|
|---|---|
|border|简写属性，用于把针对四个边的属性设置在一个声明。|
|border-style|用于设置元素所有边框的样式，或者单独地为各边设置边框样式。|
|border-width|简写属性，用于为元素的所有边框设置宽度，或者单独地为各边边框设置宽度。|
|border-color|简写属性，设置元素的所有边框中可见部分的颜色，或为 4 个边分别设置颜色。|
|border-bottom|简写属性，用于把下边框的所有属性设置到一个声明中。|
|border-bottom-color|设置元素的下边框的颜色。|
|border-bottom-style|设置元素的下边框的样式。|
|border-bottom-width|设置元素的下边框的宽度。|
|border-left|简写属性，用于把左边框的所有属性设置到一个声明中。|
|border-left-color|设置元素的左边框的颜色。|
|border-left-style|设置元素的左边框的样式。|
|border-left-width|设置元素的左边框的宽度。|
|border-right|简写属性，用于把右边框的所有属性设置到一个声明中。|
|border-right-color|设置元素的右边框的颜色。|
|border-right-style|设置元素的右边框的样式。|
|border-right-width|设置元素的右边框的宽度。|
|border-top|简写属性，用于把上边框的所有属性设置到一个声明中。|
|border-top-color|设置元素的上边框的颜色。|
|border-top-style|设置元素的上边框的样式。|
|border-top-width|设置元素的上边框的宽度。|

## 14. CSS 轮廓
轮廓（outline）是绘制于元素周围的一条线，位于边框边缘的外围，可起到突出元素的作用。
![轮廓](./box_outline.gif)
* 所有轮廓属性，见[菜鸟教程](http://www.runoob.com/css/css-outline.html)
* 用户笔记:
    * 1.outline是不占空间的，既不会增加额外的width或者height（这样不会导致浏览器渲染时出现reflow或是repaint）
    * 2.outline有可能是非矩形的（火狐浏览器下）

## 15. CSS 外边距(margin)
|属性|描述|
|---|---|
|margin|简写属性。在一个声明中设置所有外边距属性。|
|margin-bottom|设置元素的下外边距。|
|margin-left|设置元素的左外边距。|
|margin-right|设置元素的右外边距。|
|margin-top|设置元素的上外边距。|

## 16. CSS 填充(padding)
|属性|说明|
|---|---|
|padding|使用简写属性设置在一个声明中的所有填充属性|
|padding-bottom|设置元素的底部填充|
|padding-left|设置元素的左部填充|
|padding-right|设置元素的右部填充|
|padding-to|p设置元素的顶部填充|

## 17. CSS 分组和嵌套选择器
### 17.1 分组选择器
* `,`分割
```css
h1,h2,p
{
    color:green;
}
```

### 17.2 嵌套选择器
* p{ }: 为所有 p 元素指定一个样式。
* .marked{ }: 为所有 class="marked" 的元素指定一个样式。
* .marked p{ }: 为所有 class="marked" 元素内的 p 元素指定一个样式。
* p.marked{ }: 为所有 class="marked" 的 p 元素指定一个样式。
* 示例见：[5_ex](./Examples/5_ex_nested_selector.html)

## 18. CSS 尺寸 (Dimension)
|属性|描述|
|---|---|
|height|设置元素的高度。|
|line-height|设置行高。|
|max-height|设置元素的最大高度。|
|max-width|设置元素的最大宽度。|
|min-height|设置元素的最小高度。|
|min-width|设置元素的最小宽度。|
|width|设置元素的宽度。|

## 19. CSS 定位(Position)
* static定位：HTMK元素的默认值；静态定位的元素不受top、bottom、left、right的影响；
* fixed定位：元素的位置相对于浏览器窗口固定，即使窗口是滚动的，它也不会移动；
* relative定位：相对定位元素的定位是相对其正常位置。【正常位置？！】
* absolute定位：绝对定位的元素的位置相对于最近的已定位父元素，如果元素没有已定位的父元素，那么它的位置相对于<html>；
* sticky定位：粘性定位的元素是依赖于用户的滚动；
    * 跨越特定阈值前为相对定位，之后为固定定位。
        * 特定阈值指的是 top, right, bottom 或 left 之一
    * 它的行为就像 position:relative; 而当页面滚动超出目标区域时，它的表现就像 position:fixed;，它会固定在目标位置。
见：[所有的CSS定位属性](http://www.runoob.com/css/css-positioning.html)

## 20. CSS Overflow
* overflow 属性用于控制内容溢出元素框时显示的方式，默认：visible。

|值|描述|
|---|---|
|visible|默认值。内容不会被修剪，会呈现在元素框之外。|
|hidden|内容会被修剪，并且其余内容是不可见的。|
|scroll|内容会被修剪，但是浏览器会显示滚动条以便查看其余的内容。|
|auto|如果内容被修剪，则浏览器会显示滚动条以便查看其余的内容。|
|inherit|规定应该从父元素继承 overflow 属性的值。|

## 21. CSS 组合选择符
组合选择符说明了两个选择器直接的关系。
在 CSS3 中包含了四种组合方式:
* 后代选择器(以空格分隔): 选取某元素的后代元素。
* 子元素选择器(以大于号分隔）: 选择作为某元素子元素的元素。
* 相邻兄弟选择器（以加号分隔）: 选择紧接在另一元素后的元素，且二者有相同父元素
* 普通兄弟选择器（以破折号分隔）: 选取所有指定元素之后的相邻兄弟元素。

## 22. CSS 伪类(Pseudo-classes)
|选择器|示例|示例说明|
|---|---|---|
|:checked|input:checked|选择所有选中的表单元素|
|:disabled|input:disabled|选择所有禁用的表单元素|
|:empty|p:empty|选择所有没有子元素的p元素|
|:enabled|input:enabled|选择所有启用的表单元素|
|:first-of-type	p:first-of-type|选择的每个 p 元素是其父元素的第一个 p 元素|
|:in-range|input:in-range|选择元素指定范围内的值|
|:invalid|input:invalid|选择所有无效的元素|
|:last-child|p:last-child|选择所有p元素的最后一个子元素|
|:last-of-type|p:last-of-type|选择每个p元素是其母元素的最后一个p元素|
|:not(selector)|:not(p)|选择所有p以外的元素|
|:nth-child(n)|p:nth-child(2)|选择所有 p 元素的父元素的第二个子元素|
|:nth-last-child(n)|p:nth-last-child(2)|选择所有p元素倒数的第二个子元素|
|:nth-last-of-type(n)|p:nth-last-of-type(2)|选择所有p元素倒数的第二个为p的子元素|
|:nth-of-type(n)|p:nth-of-type(2)|选择所有p元素第二个为p的子元素|
|:only-of-type|p:only-of-type|选择所有仅有一个子元素为p的元素|
|:only-child|p:only-child|选择所有仅有一个子元素的p元素|
|:optional|input:optional|选择没有"required"的元素属性|
|:out-of-range|input:out-of-range|选择指定范围以外的值的元素属性|
|:read-only|input:read-only|选择只读属性的元素属性|
|:read-write|input:read-write|选择没有只读属性的元素属性|
|:required|input:required|选择有"required"属性指定的元素属性|
|:root|root|选择文档的根元素|
|:target|#news:target|选择当前活动#news元素(点击URL包含锚的名字)|
|:valid|input:valid|选择所有有效值的属性|
|:link|a:link|选择所有未访问链接|
|:visited|a:visited|选择所有访问过的链接|
|:active|a:active|选择正在活动链接|
|:hover|a:hover|把鼠标放在链接上的状态|
|:focus|input:focus|选择元素输入后具有焦点|
|:first-letter|p:first-letter|选择每个<p> 元素的第一个字母|
|:first-line|p:first-line|选择每个<p> 元素的第一行|
|:first-child|p:first-child|选择器匹配属于任意元素的第一个子元素的 <p> 元素|
|:before|p:before|在每个<p>元素之前插入内容|
|:after|p:after|在每个<p>元素之后插入内容|
|:lang(language)|p:lang(it)|为<p>元素的lang属性选择一个开始值|


