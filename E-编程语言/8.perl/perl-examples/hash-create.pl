#!/usr/bin/perl
 
# 1
%data = ('google', 'google.com', 'runoob', 'runoob.com', 'taobao', 'taobao.com');
 
print "\$data{'google'} = $data{'google'}\n";
print "\$data{'runoob'} = $data{'runoob'}\n";
print "\$data{'taobao'} = $data{'taobao'}\n";

# 2
$data{'google'} = 'google.com';
$data{'runoob'} = 'runoob.com';
$data{'taobao'} = 'taobao.com';

%data = ('google', 'google.com', 'runoob', 'runoob.com', 'taobao', 'taobao.com');

# 3
%data = ('google'=>'google.com', 'runoob'=>'runoob.com', 'taobao'=>'taobao.com');
print "\$data{'google'} = $data{'google'}\n";
print "\$data{'runoob'} = $data{'runoob'}\n";
print "\$data{'taobao'} = $data{'taobao'}\n";

# 4
%data = (-google=>'google.com', -runoob=>'runoob.com', -taobao=>'taobao.com');
