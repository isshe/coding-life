#!/usr/bin/perl

=pod
warn：触发警告输出到 STDERR
die：结束程序
carp：输出程序的跟踪信息到 STDERR
cluck：打印堆栈
croak：结束程序，并打印输出
confess：结束程序，打印堆栈
=cut

# die and unless
die "Error: 无法打开目录!: $!" unless(chdir("/tmp"));

# 

package T;

require Exporter;
@ISA = qw/Exporter/;
@EXPORT = qw/function/;
use Carp;

# carp
sub function {
    carp "---isshe---: Error in module!";
}

use T;
function();