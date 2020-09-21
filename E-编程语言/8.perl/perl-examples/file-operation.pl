#!/usr/bin/perl
 
open(DATA, "<format.pl") or die "format.pl 文件无法打开, $!";
 
while(<DATA>){
   print "$_";
}

close(DATA) || die "无法关闭文件";

# 标准输入
print "请输入：\n";
$name = <STDIN>;
print "输入内容：$name\n";


# lines
open(DATA,"<format.pl") or die "无法打开数据";
@lines = <DATA>;
print @lines;    # 输出数组内容
close(DATA);

# rename
rename ("/usr/runoob/test/file1.txt", "/usr/runoob/test/file2.txt" );

# delete
unlink ("/usr/runoob/test/file1.txt");

# file info
my $file = "/usr/test/runoob/file1.txt";
my (@description, $size);
if (-e $file)
{
    push @description, '是一个二进制文件' if (-B _);
    push @description, '是一个socket(套接字)' if (-S _);
    push @description, '是一个文本文件' if (-T _);
    push @description, '是一个特殊块文件' if (-b _);
    push @description, '是一个特殊字符文件' if (-c _);
    push @description, '是一个目录' if (-d _);
    push @description, '文件存在' if (-x _);
    push @description, (($size = -s _)) ? "$size 字节" : '空';
    print "$file 信息：", join(', ',@description),"\n";
}