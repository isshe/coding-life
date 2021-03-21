#!/usr/bin/perl

$scalar = "Hello1";
@scalar = ("Hello2", "World", 123);
$refer = "scalar";   # 引用变量名

$a = $$refer;        #
$b = $$refer[1];     #

print "$a";
print "$b";

