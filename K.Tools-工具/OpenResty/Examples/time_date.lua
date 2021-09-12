#!/usr/local/openresty/bin/resty

ngx.say(ngx.today())
ngx.say(ngx.localtime())
ngx.say(ngx.utctime())
ngx.say(ngx.now())
ngx.say(ngx.time())

ngx.say(ngx.http_time(ngx.time()))
ngx.say(ngx.cookie_time(ngx.time()))
ngx.say(ngx.parse_http_time(ngx.http_time(ngx.time())))