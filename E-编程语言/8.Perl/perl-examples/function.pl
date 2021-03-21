#!/usr/bin/perl


=pod
sub subroutine{
   statements;
}
=cut


# 函数定义
sub Hello{
   print "Hello, World!\n";
}

# 定义求平均值函数
sub Average{
   # 获取所有传入的参数
   $n = scalar(@_);
   $sum = 0;
 
   foreach $item (@_){
      $sum += $item;
   }
   $average = $sum / $n;
   print '传入的参数为 : ',"@_\n";           # 打印整个数组
   print "第一个参数值为 : $_[0]\n";         # 打印第一个参数
   print "传入参数的平均值为 : $average\n";  # 打印平均值
}

# 定义函数
sub PrintList{
   my @list = @_;
   print "列表为 : @list\n";
   print "第一个参数值为 : $_[0]\n";
}

sub PrintHash{
   my (%hash) = @_;
 
   foreach my $key ( keys %hash ){
      my $value = $hash{$key};
      print "$key : $value\n";
   }
}

# 如果没有使用 return 语句，则子程序的最后一行语句将作为返回值
sub add_a_b{
   # 不使用 return
   $_[0]+$_[1];  
 
   # 使用 return
   # return $_[0]+$_[1];  
}
 

# 函数调用
Hello();

# 调用函数
Average(10, 20, 30);

# 列表参数
$a = 10;
@b = (1, 2, 3, 4);
PrintList($a, @b);

 
# 传递哈希
%hash = ('name' => 'runoob', 'age' => 3);
PrintHash(%hash);


# 返回值
print add_a_b(1, 2)