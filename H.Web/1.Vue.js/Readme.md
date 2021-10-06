

# Vue.js

## 1. v-bind
* 绑定元素特性。

```html
<img v-bind:src="imgURL">
<img :src="imgURL">
<a :href="ahref">百度一下</a>
<!-- obj= {active: isActive, test: isTest} -->
<div :class="{'active': isActive, 'test': isTest}"></div>
<div :class="obj"></div>
<!-- array=["active", "test"] -->
<div :class="['active', 'test']"></div>
<div :class="array"></div>
<div class="abc" :class="array"></div>
```

## 2. v-if
* 条件判断。

## 3. v-for
* 循环。

```
<li v-for="m in movies"></li>
<li v-for="(m,index) in movies"></li>
```

## 4. v-on

> @click

* 添加事件监听器。

```
v-on:click
```

## 5. v-model
* 实现表单和应用状态之间的双向绑定。

```html
<div id="app">
    <h2>{{message}}</h2>
    <input type="text" v-model="message">
    <!-- 等价于 -->
    <input type="text" :value="message" v-on:input="message = $event.target.value">
</div>
```

## 6. component

## 7. 实例的生命周期
![生命周期](./lifecycle.png)

## 什么是 MVVM？
model、view、viewmodel

## v-once
* 只渲染一次

```html
<h2 v-once>{{message}}</h2>
```

## v-html
* 渲染成 html

```html
<!-- url = '<a href="http://baidu.com">百度一下<a/>' -->
<h2 v-html="url"><h2>
```

## v-pre
* 跳过此元素和它的子元素的编程过程。

```html
<!-- message = "abc"; -->
<h2 v-pre>{{message}}<h2>
<!-- 原样显示 -->
```

## v-cloak
* 延后展示

```html
<div id="app" v-cloak>
    {{message}}
</div>
```
