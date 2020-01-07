var http = require("http")

http.createServer(function(request, response){
    response.writeHead(200, {
        'Content-Type': 'text/plain'
    });
    response.end('Hello world\n');
}).listen(48888);

console.log('Server running at http://127.0.0.1:48888/');
