#!/usr/bin/perl
 
$a = 10;
 
$b = q{a = $a}; # 'a = $a'
print "q{a = \$a} = $b\n";
 
$b = qq{a = $a};    # "a = $a"
print "qq{a = \$a} = $b\n";
 
# 使用 unix 的 date 命令执行
$t = qx{date};  # `date`
print "qx{date} = $t\n";