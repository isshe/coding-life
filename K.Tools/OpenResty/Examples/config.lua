#!/usr/local/openresty/bin/resty


ngx.say(ngx.config.debug)            -- 是否是 debug 版本
ngx.say(ngx.config.prefix())         -- 安装目录
ngx.say(ngx.config.nginx_version)    -- nginx 版本, 如 1015008 -> 1.015.008 -> 1.15.8
ngx.say(ngx.config.nginx_configure())  -- 编译选项
ngx.say(ngx.config.subsystem)        -- 所在子系统名称
ngx.say(ngx.config.ngx_lua_version)  -- 所在子系统版本号，如 10015 -> 0.010.015 -> 0.10.15