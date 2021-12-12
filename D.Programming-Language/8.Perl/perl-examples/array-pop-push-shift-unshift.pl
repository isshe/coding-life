#!/usr/bin/perl
 
# 创建一个简单是数组
@sites = ("google","runoob","taobao");
$new_size = @sites ;
print "1. \@sites  = @sites\n"."原数组长度 ：$new_size\n";
# 在数组结尾添加一个元素
$new_size = push(@sites, "baidu");
print "2. \@sites  = @sites\n"."新数组长度 ：$new_size\n";
 
# 在数组开头添加一个元素
$new_size = unshift(@sites, "weibo");
print "3. \@sites  = @sites\n"."新数组长度 ：$new_size\n";
 
# 删除数组末尾的元素
$new_byte = pop(@sites);
print "4. \@sites  = @sites\n"."弹出元素为 ：$new_byte\n";
 
# 移除数组开头的元素
$new_byte = shift(@sites);
print "5. \@sites  = @sites\n"."弹出元素为 ：$new_byte\n";