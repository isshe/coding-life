#!/usr/bin/perl
 
%data = ('google'=>'google.com', 'runoob'=>'runoob.com', 'taobao'=>'taobao.com');
while(($key, $value) = each(%data)){
    print "$data{$key}\n";
}