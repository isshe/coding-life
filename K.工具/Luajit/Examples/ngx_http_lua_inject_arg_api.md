# 栈变化示例


```lua
ngx_http_lua_inject_arg_api(lua_State *L)
{
    lua_pushliteral(L, "arg");
    lua_newtable(L);    /*  .arg table aka {} */

    lua_createtable(L, 0 /* narr */, 2 /* nrec */);    /*  the metatable */

    lua_pushcfunction(L, ngx_http_lua_param_get);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, ngx_http_lua_param_set);
    lua_setfield(L, -2, "__newindex");

    lua_setmetatable(L, -2);    /*  tie the metatable to param table */

    dd("top: %d, type -1: %s", lua_gettop(L), luaL_typename(L, -1));

    // ngx
    lua_rawset(L, -3);    /*  set ngx.arg table */
}
```

堆栈示意：

`:ngx`：表示这个表以 ngx 名称表示

```
                                                  [  { }  ]:meta
                                [  { }  ]:arg     [  { }  ]:arg
              [ "arg" ]         [ "arg" ]         [ "arg" ]
[ {} ]:ngx    [  { }  ]:ngx     [  { }  ]:ngx     [  { }  ]:ngx
初始状态 --> lua_pushliteral --> lua_newtable --> lua_createtable

=>

[ngx_http_lua_param_get]
      [  { }  ]:meta                  [ {__index = ngx_http_lua_param_get }]:meta
      [  { }  ]:arg                   [  { }  ]:arg
      [ "arg" ]                       [ "arg" ]
      [  { }  ]:ngx                   [  { }  ]:ngx
 lua_pushcfunction      -->          lua_setfield

=>

[ngx_http_lua_param_set]
[ {__index = ngx_http_lua_param_get} ] [ {__index = ngx_http_lua_param_get,
                                         __newindex = ngx_http_lua_param_set} ]
      [  { }  ]:arg                           [  { }  ]:arg
      [ "arg" ]                               [ "arg" ]
      [  { }  ]:ngx                           [  { }  ]:ngx
 lua_pushcfunction            -->            lua_setfield

=>

       [  { }  ]:arg metable = [ {__index = ngx_http_lua_param_get,
                                __newindex = ngx_http_lua_param_set} ]:meta
       [ "arg" ]
       [  { }  ]:ngx
     lua_setmetatable

=>
    [ { arg = :arg } ]:ngx
```

逐句分析：

- lua_pushliteral：压栈字符串 "arg"
- lua_newtable：创建 table 并压栈
- lua_createtable：创建 table 并压栈，参数 0 表示数组长度，2 表示哈希表长度
- lua_pushcfunction：把 C 函数 ngx_http_lua_param_get 压栈
- lua_setfield：取 -2 位置的值 meta，设置 meta["__index"] = 栈顶元素（也就是 ngx_http_lua_param_get），然后把栈顶元素出栈
- lua_pushcfunction：把 C 函数 ngx_http_lua_param_set 压栈
- lua_setfield：取 -2 位置的值 meta，设置 meta["__newindex"] = 栈顶元素（也就是 ngx_http_lua_param_set），然后把栈顶元素出栈
- lua_setmetatable：出栈栈顶，并把出栈的元素设置成 -2 位置的元表（metatable）
- lua_rawset：设置 -3 位置（ngx）的内容，-2 位置为 key，-1 位置为 value，也就是 ngx[key] = value，然后出栈 key 和 value
