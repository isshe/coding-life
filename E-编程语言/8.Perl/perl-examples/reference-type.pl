#!/usr/bin/perl
 
$var = 10;
$r = \$var;
print "r 的引用类型 : ", ref($r), "\n";
 
@var = (1, 2, 3);
$r = \@var;
print "r 的引用类型 : ", ref($r), "\n";
 
%var = ('key1' => 10, 'key2' => 20);
$r = \%var;
print "r 的引用类型 : ", ref($r), "\n";