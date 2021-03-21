#!/usr/bin/perl

%data = (-taobao => 45, -google => 30, -runoob => 40);
 
@array = @data{-taobao, -runoob};
 
print "Array : @array\n";

%data = ('google'=>'google.com', 'runoob'=>'runoob.com', 'taobao'=>'taobao.com');
 
@urls = values %data;
 
print "$urls[0]\n";
print "$urls[1]\n";
print "$urls[2]\n";