#!/usr/bin/perl
 
print "文件名 ". __FILE__ . "\n";
print "行号 " . __LINE__ ."\n";
print "包名 " . __PACKAGE__ ."\n";
 
# 无法解析
print "__FILE__ __LINE__ __PACKAGE__\n";