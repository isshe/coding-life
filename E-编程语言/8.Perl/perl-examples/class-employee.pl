#!/usr/bin/perl
 
use Person;
 
$object = new Person( "小明", "王", 23234345);
# 获取姓名
$firstName = $object->getFirstName();
 
print "设置前姓名为 : $firstName\n";
 
# 使用辅助函数设置姓名
$object->setFirstName( "小强" );
 
# 通过辅助函数获取姓名
$firstName = $object->getFirstName();
print "设置后姓名为 : $firstName\n";