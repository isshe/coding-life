--- Copyright 2023 OpenResty Inc.
-- @author: isshe (i.sshe@outlook.com)

local _M = {}

local ngx = ngx
local ffi = require("ffi")
local base = require ("resty.core.base")
local C = ffi.C
local get_request = base.get_request
local NGX_ERR = ngx.ERROR

ffi.cdef[[
int ngx_http_lua_highcpu_test(ngx_http_request_t *r);
]]


local function run()
    local r = get_request()
    if not r then
        error("no request found")
    end

    local rc = C.ngx_http_lua_highcpu_test(r)

    if rc == NGX_ERR then
        return nil, "parse highcpu files failed"
    else
        return true
    end
end
_M.run = run


return _M
