#!/usr/bin/perl

# endless loop
print("------------endless loop(for)--------------\n");

my $i = 0;
for( ; ; )
{
   printf "循环会无限执行。\n";
   next if $i < 0;
   last if $i == 0;
}


# endless loop2
print("------------endless loop2(while)--------------\n");
my $i = 0;
while(1)
{
   printf "循环会无限执行。\n";
   next if $i < 0;
   last if $i == 0;
}


# foreach array
print("------------foreach array--------------\n");
my @arr = (1, 2, 3);
foreach my $n (@arr) {
   print("$n, ")
}
print("\n");


# foreach array
print("------------foreach array 2--------------\n");
my @arr = (1, 2, 3);
foreach (@arr) {
   print("$_, ")  # 注意这里
}
print("\n");


# foreach hash
print("------------foreach hash--------------\n");
my %hash = (two => 2, three => 3, four => 4, five => 5);
foreach my $n (keys %hash) {
   print("$n: $hash{$n}\n")   # 注意这里，不能用 %hash{$n}
}


# for
print("------------for--------------\n");
for (my $i = 0; $i < 10; $i++) {
   print("$i, ");
}
print("\n");


# while
print("------------while--------------\n");
my $i = 0;
while ($i < 10) {
   print("$i, ");
   $i++;
}
print("\n");


# until
print("------------until--------------\n");
my $i = 0;
until ($i >= 10) {   # 注意这里
   print("$i, ");
   $i++;
}
print("\n");
