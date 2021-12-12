#!/usr/bin/perl


# 全局变量
$string = "Hello, World!";
 
# 函数定义
sub PrintHello{
   # PrintHello 函数的私有变量
   my $string;
   $string = "Hello, Runoob!";
   print "函数内字符串：$string\n";
   PrintMe();
}

sub PrintRunoob{
   # PrintHello 函数私有变量
   local $string;
   $string = "Hello, Runoob!";
   # 子程序调用的子程序
   PrintMe();
   print "PrintRunoob 函数内字符串值：$string\n";
}
sub PrintMe{
   print "PrintMe 函数内字符串值：$string\n";
}
 
sub PrintHello2{
   print "PrintHello2 函数内字符串值：$string\n";
}


# 调用函数
PrintHello();
print "函数外字符串：$string\n";

# 函数调用
PrintRunoob();
PrintHello2();
print "函数外部字符串值：$string\n";