#!/usr/bin/perl


=pod
格式：
format FormatName =
fieldline
value_one, value_two, value_three
fieldline
value_one, value_two
.
=cut
format EMPLOYEE =
===================================
@<<<<<<<<<<<<<<<<<<<<<< @<< 
$name, $age
@#####.##
$salary
===================================
.
 
# 添加分页 $% 
format EMPLOYEE_TOP =
===================================
Name                    Age Page @<
                                 $%
=================================== 
.
 
select(STDOUT);
$~ = EMPLOYEE;
$^ = EMPLOYEE_TOP;  # 设置表头
 
@n = ("Ali", "Runoob", "Jaffer");
@a  = (20,30, 40);
@s = (2000.00, 2500.00, 4000.000);
 
$i = 0;
foreach (@n){
   $name = $_;
   $age = $a[$i];
   $salary = $s[$i++];
   write;
}