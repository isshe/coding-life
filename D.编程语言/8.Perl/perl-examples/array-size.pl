#!/usr/bin/perl
 
@names = ('google', 'runoob', 'taobao');
 
@copy = @names;   # 复制数组
$size = @names;   # 数组赋值给标量，返回数组元素个数
 
print "名字为 : @copy\n";
print "名字数为 : $size\n";