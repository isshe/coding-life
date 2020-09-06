#!/usr/bin/perl

@array = (1, 2, 'Hello');
@sites = qw/google taobao runoob/;
 
print "$sites[0]\n";
print "$sites[1]\n";
print "$sites[2]\n";
print "$sites[-1]\n";    # 负数，反向读取

 
@var_10 = (1..10);
@var_20 = (10..20);
@var_abc = (a..z);
 
print "@var_10\n";   # 输出 1 到 10
print "@var_20\n";   # 输出 10 到 20
print "@var_abc\n";  # 输出 a 到 z