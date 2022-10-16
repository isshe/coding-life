var fs = require("fs");

var data = "isshe"

var writerStream = fs.createWriteStream('output.tmp');

// 使用utf8编码写入数据
writerStream.write(data, 'UTF8');
writerStream.end();

writerStream.on('finish', function() {
    console.log("写入完成");
});

writerStream.on('error', function(err) {
    console.log(err.stack);
});

console.log("程序执行完成");