#!/usr/bin/perl
 
# 定义数组
@sites = qw(google taobao runoob facebook);
print "网站: @sites\n";
 
# 设置数组的第一个索引为 1
$[ = 1;
 
print "\@sites[1]: $sites[1]\n";
print "\@sites[2]: $sites[2]\n";