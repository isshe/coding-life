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

# Buffer
> Buffer是类。
> buf是变量/实例。

```js
// 创建
Buffer.alloc(size[, fill[, encoding]])： 返回一个指定大小的 Buffer 实例，如果没有设置 fill，则默认填满 0
Buffer.allocUnsafe(size)： 返回一个指定大小的 Buffer 实例，但是它不会被初始化，所以它可能包含敏感的数据
Buffer.allocUnsafeSlow(size)
Buffer.from(array)： 返回一个被 array 的值初始化的新的 Buffer 实例（传入的 array 的元素只能是数字，不然就会自动被 0 覆盖）
Buffer.from(arrayBuffer[, byteOffset[, length]])： 返回一个新建的与给定的 ArrayBuffer 共享同一内存的 Buffer。
Buffer.from(buffer)： 复制传入的 Buffer 实例的数据，并返回一个新的 Buffer 实例
Buffer.from(string[, encoding])： 返回一个被 string 的值初始化的新的 Buffer 实例

// 写入
buf.write(string[, offset[, length]][, encoding])

// 读取
buf.toString([encoding[, start[, end]]])

// 转json
buf.toJSON()

// 比较
buf.compare(otherBuffer);

// 合并
Buffer.concat(list[, totalLength])

// 复制
buf.copy(targetBuffer[, targetStart[, sourceStart[, sourceEnd]]])
// targetBuffer - 要拷贝的 Buffer 对象。
// targetStart - 数字, 可选, 默认: 0
// sourceStart - 数字, 可选, 默认: 0
// sourceEnd - 数字, 可选, 默认: buffer.length

// 裁剪
buf.slice([start[, end]])

// 缓冲区长度
buf.length;
```

# Stream
> 示例:
> [stream_write.js](./examples/stream_write.js)
> [stream_read.js](./examples/stream_read.js)

nodejs流的类型:
* Readable: 可读
* Writable：可写
* Duplex：可读可写
* Transform：先写后读。

所有的 Stream 对象都是 EventEmitter 的实例。常用的事件有：
* data - 当有数据可读时触发。
* end - 没有更多的数据可读时触发。
* error - 在接收和写入过程中发生错误时触发。
* finish - 所有数据已被写入到底层系统时触发。

## 管道流
```js
var fs = require("fs");

// 创建一个可读流
var readerStream = fs.createReadStream('input.txt');

// 创建一个可写流
var writerStream = fs.createWriteStream('output.txt');

// 管道读写操作
// 读取 input.txt 文件内容，并将内容写入到 output.txt 文件中
readerStream.pipe(writerStream);
```

## 链式流
```js
var fs = require("fs");
var zlib = require('zlib');

// 压缩 input.txt 文件为 input.txt.gz
fs.createReadStream('input.txt')
  .pipe(zlib.createGzip())
  .pipe(fs.createWriteStream('input.txt.gz'));

console.log("文件压缩完成。");
```

# 模块系统
exports：导出
require：引入

# 全局对象/变量
```
__filename: 当前正在执行的脚本的文件名。
__dirname: 当前执行脚本所在的目录。
```

## console
```
console.trace();
console.log();
```

## process
用于描述当前Node.js 进程状态的对象；提供了一个与操作系统交互的简单接口。
```
process.on('exit', callback_func);
process.exit();
process.abort();
process.getpid();
```

# fs
> 详见：https://nodejs.org/api/fs.html#fs_fs_rename_oldpath_newpath_callback
> 示例：./examples/fs.js
```js
var fs = require("fs")

// 打开
fs.open(path, flags[, mode], callback)
// 获取文件信息
fs.stat(path, callback)
// 写
fs.writeFile(file, data[, options], callback)
// 写
fs.write(fd, buffer[, offset[, length[, position]]], callback)
// 读
fs.read(fd, buffer, offset, length, position, callback)
// 关闭
fs.close(fd, callback
// 截断
fs.ftruncate(fd, len, callback)
// 删除
fs.unlink(path, callback)
// 创建目录
fs.mkdir(path[, options], callback)
// 读取目录
fs.readdir(path, callback)
// 删除目录
fs.rmdir(path, callback)
```
