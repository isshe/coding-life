#!/usr/bin/perl
 
=pod
sec,     # 秒， 0 到 61
min,     # 分钟， 0 到 59
hour,    # 小时， 0 到 24
mday,    # 天， 1 到 31
mon,     # 月， 0 到 11
year,    # 年，从 1900 开始
wday,    # 星期几，0-6,0表示周日
yday,    # 一年中的第几天,0-364,365
isdst    # 如果夏令时有效，则为真
=cut

@months = qw( 一月 二月 三月 四月 五月 六月 七月 八月 九月 十月 十一月 十二月 );
@days = qw(星期天 星期一 星期二 星期三 星期四 星期五 星期六);
 
($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
print "$mday $months[$mon] $days[$wday]\n";

# 
$datestring = localtime();
print "时间日期为：$datestring\n";

# 格林威治时间
$gmt_datestring = gmtime();
print "GMT 时间日期为：$gmt_datestring\n";

# 
($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime();
printf("格式化时间：HH:MM:SS\n");
printf("%02d:%02d:%02d", $hour, $min, $sec);

# epoch time
$epoc = time();
print "从1970年1月1日起累计的秒数为：$epoc\n";

# 格式化时间
use POSIX qw(strftime);
 
$datestring = strftime "%Y-%m-%d %H:%M:%S", localtime;
printf("时间日期 - $datestring\n");
 
#  GMT 格式化时间日期
$datestring = strftime "%Y-%m-%d %H:%M:%S", gmtime;
printf("时间日期 - $datestring\n");