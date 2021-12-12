#!/usr/bin/perl
 
%data = ('google'=>'google.com', 'runoob'=>'runoob.com', 'taobao'=>'taobao.com');
 
@keys = keys %data;
$size = @keys;
print "1 - 哈希大小: $size\n";
 
@values = values %data;
$size = @values;
print "2 - 哈希大小: $size\n";