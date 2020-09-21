#!/usr/bin/perl

=pod
opendir DIRHANDLE, EXPR  # 打开目录
readdir DIRHANDLE        # 读取目录
rewinddir DIRHANDLE      # 定位指针到开头
telldir DIRHANDLE        # 返回目录的当前位置
seekdir DIRHANDLE, POS   # 定位指定到目录的 POS 位置
closedir DIRHANDLE       # 关闭目录
mkdir DIRNAME            # 创建目录
rmdir DIRNAME            # 删除目录
chdir DIRNAME            # 切换目录
=cut

# 显示 /tmp 目录下的所有文件
$dir = "/tmp/*";
my @files = glob( $dir );
 
foreach (@files ){
   print $_ . "\n";
}
 
# 显示 /tmp 目录下所有以 .c 结尾的文件
$dir = "/tmp/*.c";
@files = glob( $dir );
 
foreach (@files ){
   print $_ . "\n";
}
 
# 显示所有隐藏文件
$dir = "/tmp/.*";
@files = glob( $dir );
foreach (@files ){
   print $_ . "\n";
}
 
# 显示 /tmp 和 /home 目录下的所有文件
$dir = "/tmp/* /home/*";
@files = glob( $dir );
 
foreach (@files ){
   print $_ . "\n";
}

# 列出当前目录下的所有文件
opendir (DIR, '.') or die "无法打开目录, $!";
while ($file = readdir DIR) {
  print "$file\n";
}
closedir DIR;

# 列出/tmp 目录下所有.c 结尾的文件
opendir(DIR, '.') or die "无法打开目录, $!";
foreach (sort grep(/^.*\.c$/,readdir(DIR))){
   print "$_\n";
}
closedir DIR;