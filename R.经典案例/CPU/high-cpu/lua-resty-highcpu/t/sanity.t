# vim:set ft= ts=4 sw=4 et fdm=marker:

use Test::Nginx::Socket::Lua;
use Cwd qw(cwd);
use File::Path qw(make_path remove_tree);

repeat_each(2);

$ENV{TEST_NGINX_HTML_DIR} = html_dir;

plan tests => repeat_each() * (blocks() + 3);

no_diff();
no_shuffle();
#no_long_string();

my $pwd = cwd();

our $HttpConfig = <<"_EOC_";
    lua_package_path "$pwd/lib/?.lua;$pwd/lua-resty-core/lib/?.lua;;";
_EOC_

run_tests();

__DATA__

=== TEST 1: sanity
--- http_config eval: $::HttpConfig
--- config
    location = /t {
        content_by_lua_block {
            local highcpu = require("resty.highcpu")

            local ok, err = highcpu.run()
            ngx.say(ok)
        }
    }
--- request
GET /t
--- response_body
true
--- no_error_log
[alert]
--- error_log
running high cpu test
