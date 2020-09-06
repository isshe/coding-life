#!/usr/bin/perl
 
# split [ PATTERN [ , EXPR [ , LIMIT ] ] ]

# 定义字符串
$var_test = "runoob";
$var_string = "www-runoob-com";
$var_names = "google,taobao,runoob,weibo";
 
# 字符串转为数组
@test = split('', $var_test);
@string = split('-', $var_string);
@names  = split(',', $var_names);

print "$test[3]\n";  # 输出 o
print "$string[2]\n";  # 输出 com
print "$names[3]\n";   # 输出 weibo