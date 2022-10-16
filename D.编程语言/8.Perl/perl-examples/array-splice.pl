#!/usr/bin/perl
 
@nums = (1..20);
print "替换前 - @nums\n";
 
splice(@nums, 0, 5, 21..25); 
print "替换后 - @nums\n";