#!/usr/bin/perl
 
use feature 'state';
 
sub PrintCount{
   state $count = 0; # 初始化变量
 
   print "counter 值为：$count\n";
   $count++;
}
 
for (1..5){
   PrintCount();
}