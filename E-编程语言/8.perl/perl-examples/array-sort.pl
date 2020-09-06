#!/usr/bin/perl
 
# 定义数组
@sites = qw(google taobao runoob facebook);
print "排序前: @sites\n";
 
# 对数组进行排序
@sites = sort(@sites);
print "排序后: @sites\n";