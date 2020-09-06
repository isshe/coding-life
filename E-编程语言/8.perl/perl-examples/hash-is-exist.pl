#!/usr/bin/perl
 
%data = ('google'=>'google.com', 'runoob'=>'runoob.com', 'taobao'=>'taobao.com');
 
if( exists($data{'facebook'} ) ){
   print "facebook 的网址为 $data{'facebook'} \n";
}
else
{
   print "facebook 键不存在\n";
}