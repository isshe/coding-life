[TOC]

nodejs
---

# 安装
```shell
# https://nodejs.org/en/download/，到此网址下载安装包即可

# npm升级
sudo npm install npm -g

# 安装模块, -g: 全局安装
npm install <module> [-g]

# 指定镜像安装
npm install <module> [-g] --registry=https://registry.npm.taobao.org

# 卸载
npm uninstall <module> [-g]

# 更新
npm update <module>

# 搜索
npm search <module>

# 查看已安装模块
npm list -g

# 查看某个已安装模块的版本号
npm list <module>
```

# package.json
* name: 包名。
* version: 包版本。
* description: 包描述。
* homepage: 包的官网URL。
* author: 包的作者。
* contributer: 贡献者。
* dependencise: 包的依赖列表。
* repository: 包代码存放的地方的类型。git/svn。
* main: 程序的入口文件。
* keywords: 关键字。

## 生成方式
```
npm init
```

# EventEmitter
**大多数时候我们不会直接使用 EventEmitter，而是在对象中继承它。**
```js
// 引入 events 模块
var events = require('events');
// 创建 eventEmitter 对象
var eventEmitter = new events.EventEmitter();

// 绑定事件
eventEmitter.on('event_name', event_handler);

// 触发事件
eventEmitter.emit('event_name', [argv1], [argv2], ...   );

// 绑定只能触发一次的事件
eventEmitter.once('event_name', event_handler);

// 删除事件
eventEmitter.removeListener('event_name', event_handler);

// 删除所有事件
eventEmitter.removeAllListeners();

// 设置最大监听事件数量
eventEmitter.setMaxListeners(n);
```

