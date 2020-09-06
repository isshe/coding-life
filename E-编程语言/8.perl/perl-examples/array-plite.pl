#!/usr/bin/perl
 
@sites = qw/google taobao runoob weibo qq facebook 网易/;
 
@sites2 = @sites[3,4,5];
 
print "@sites2\n";

@sites = qw/google taobao runoob weibo qq facebook 网易/;
 
@sites2 = @sites[3..5];
 
print "@sites2\n";