#!/usr/bin/perl
 
# 定义字符串
$var_string = "www-runoob-com";
$var_names = "google,taobao,runoob,weibo";
 
# 字符串转为数组
@string = split('-', $var_string);
@names  = split(',', $var_names);
 
 
# 数组转为字符串
$string1 = join( '-', @string );
$string2 = join( ',', @names );
 
print "$string1\n";
print "$string2\n";