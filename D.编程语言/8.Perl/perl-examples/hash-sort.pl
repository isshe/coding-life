#!/usr/bin/perl

%data = ('google'=>'google.com', 'runoob'=>'runoob.com', 'taobao'=>'taobao.com');

@names = sort {$a cmp $b} keys %data;

print "$names[0]\n";
print "$names[1]\n";
print "$names[2]\n";

@names = sort {$a <=> $b} keys %data;
print "$names[0]\n";
print "$names[1]\n";
print "$names[2]\n";