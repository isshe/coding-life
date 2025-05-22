
# OpenResty 开发指南

本文主要介绍以下两个方面的详细步骤：

- 给 Nginx 打补丁
- 开发第三方 Nginx 模块

## 给 Nginx 打补丁

### 开发

```bash
git clone https://github.com/openresty/openresty.git

cd openresty

# 下载所有依赖及打 patch 等
# 如果有一些 patch 不想要打，则编辑文件去掉
./util/mirror-tarballs

# 进入具体版本的目录，这里是生成好的代码结构，可以用于编译 OpenResty
cd openresty-VERSION

# 进行 Nginx 代码修改，编译测试
cd openresty-VERSION/bundle/nginx-VERSION
./configure && make
./sbin/nginx -p /PATH/TO/CONFIG
```

### 生成 Patch

- 方法 1：

```bash
# 把 Nginx 源码目录纳入 git 以生成 patch 文件
cd openresty
cd openresty-VERSION/bundle/nginx-VERSION
git add .

# 更新代码前
git commit -m 'change: commit 1.'

# 如果是更新 patch，则我们把更新前 patch 打上
cat OLD.patch | patch -p1

# 更新代码
...

# 如果没有新文件，可以直接使用 git diff 创建新的 patch
git diff > NEW.patch

# 如果有新文件，则创建一个 commit
git add NEW.file
git commit -m "change: commit 2."
git diff <commit id 2> <commit id 1> > NEW.patch
```

- 方法 2：

正常修改，commit 后，直接通过 commit 来生成：

```bash
# git format-patch -1 <COMMIT-ID>
git format-patch -1 HEAD~0
```

### 验证 patch

```bash
# 克隆代码仓库（包含 patch），假设是 lua-test-nginx-module 仓库（仅演示，并不存在）
git clone git@github.com:orinc/lua-test-nginx-module.git

cd lua-test-nginx-module
cp NEW.path patches/NEW.path

# 提交代码
git checkout -b isshe/update-NEW-patch
git add patches/NEW.path
git commit -m "xxx."
git push origin isshe/update-NEW-patch

# 打个 tag，从 openresty 的 util/mirror-tarballs
# 中找到 lua-test-nginx-module 的版本号，假设 v0.0.1，
# 因此我们打一个 tag：v0.0.1rc1
git tag v0.0.1rc1
git push origin v0.0.1rc1

# 回到 openresty 仓库进行验证
cd openresty

# 修改 v0.0.1 版本为 v0.0.1rc1
vi util/mirror-tarballs

# 执行 util/mirror-tarballs 更新代码
# 如果 bundle 目录或者 work 目录已经存在了相同的目录/包，则可能需要删除以便重新下载
./util/mirror-tarballs

# 编译、验证即可
cd openresty-VERSION/
./configure
make -j4

# 验证生成的 nginx 程序
./sbin/nginx -p /PATH/TO/CONFIG
```

## 开发第三方模块

```bash
git clone https://github.com/openresty/openresty.git

cd openresty

# 下载所有依赖及打 patch 等
# 如果有一些 patch 不想要打，则编辑文件去掉
./util/mirror-tarballs

# 进入具体版本的目录，这里是生成好的代码结构，可以用于编译 OpenResty
cd openresty-VERSION

# 加入我们的模块
./configure --prefix=/PATH/TO/TEST/NGINX --with-debug --add-module=/PATH/TO/YOUR/MODULE

# 编译并安装到指定目录“/PATH/TO/TEST/NGINX”
make -j4
make install

# 启动服务。假设配置目录也在 /PATH/TO/TEST/NGINX
/PATH/TO/TEST/NGINX/bin/openresty -p /PATH/TO/TEST/NGINX

# 发送请求进行测试即可
```

如果想要更快编译调试，可以把编译及之后的步骤放到新模块的 Makefile 中，如：

```makefile
update:
	cd /PATH/TO/openresty-VERSION; make -j4; make install
	/PATH/TO/TEST/NGINX/bin/openresty -p /PATH/TO/TEST/NGINX -s stop
	/PATH/TO/TEST/NGINX/bin/openresty -p /PATH/TO/TEST/NGINX
```

然后执行

```
make update
```

来更新程序。

## 更多

### 通过 FFI 接口进行调用

以下是一个简单示例：

```lua

-- some require ...

local _M = {}

local get_size_ptr = base.get_size_ptr

ffi.cdef[[
typedef struct {
    ngx_str_t                   str;
    size_t                      num;
} ngx_http_lua_test_t;

// 注意这个 API 返回的是二维数组，更简单的情况进行相应简化即可
int ngx_http_lua_tests_api(ngx_http_request_t *r,
    ngx_http_lua_test_t **out, size_t *out_len);
]]

local test_array_type = ffi.typeof("ngx_http_lua_test_t *[1]")
local test_type = ffi.typeof("ngx_http_lua_test_t *")

local function go()
    local r = get_request()
    if not r then
        error("no request found")
    end

    -- 如果 API 不可以 yield，则这个变量可以放到 go 函数外
    local test_pp = ffi_new(test_array_type)

    local sizep = get_size_ptr()
    local rc = C.ngx_http_lua_tests_api(r, test_pp, sizep)
    local count = tonumber(sizep[0]) or 0

    if rc == NGX_OK then
        local idx = 0
        local test_array = ffi_cast(test_type, test_pp[0])
        for i = 0, count - 1 do
            local info = test_array[i]

            local str = info.str
            ngx.say(ffi_str(str.data, str.len))

            ngx.say(tonumber(info.int))

        end
    else
        -- do something
    end
end
_M.go = go


return _M
```

### 复用 git tag

- 如果想要复用 tag，可以这样操作：

```bash
# 删除 tag
git tag -d v0.0.1

# 同步到云端
git push origin :refs/tags/v0.0.1

# 重新打 tag
git tag v0.0.1

# 同步到云端
git push origin v0.0.1
```
