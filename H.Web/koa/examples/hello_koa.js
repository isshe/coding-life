const Koa = require('koa');
const app = new Koa();

// response
app.use(ctx => {
  ctx.body = 'Hello Koa';
});

var port=44444
console.log("Server running at http://localhost:%d", port);
app.listen(port);