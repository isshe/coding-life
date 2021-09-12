var fs = require("fs");

var buf = new Buffer.alloc(1024);

console.log("准备打开文件");
fs.open('output.tmp', 'r+', function(err, fd) {
    if (err) {
        return console.error(err);
    }
    console.log("文件打开成功");

    console.log("准备读取文件");
    fs.read(fd, buf, 0, buf.length, 0, function(err, bytes) {
        if (err) {
            return console.error(err);
        }

        if (bytes > 0) {
            console.log("读取文件成功：" + buf.slice(0, bytes).toString());
        }
    });

    fs.close(fd, function(err) {
        if (err) {
            return console.error(err);
        }
        console.log("关闭文件成功");
    });
});