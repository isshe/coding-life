buf = Buffer.alloc(256);
len = buf.write("isshe");

console.log("len = %d", len);
console.log(buf.toString('ascii'));
console.log(buf.toString('utf8', 0, 2));
console.log(buf.toJSON());