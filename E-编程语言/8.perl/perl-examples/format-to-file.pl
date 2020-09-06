#!/usr/bin/perl
 
if (open(MYFILE, ">format-to-file.tmp")) {
$~ = "MYFORMAT";
write MYFILE; # 含文件变量的输出，此时会打印与变量同名的格式，即MYFILE。$~里指定的值被忽略。
 
format MYFILE = # 与文件变量同名 
=================================
      输入到文件中
=================================
.
close MYFILE;
}