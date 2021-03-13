#!/usr/bin/perl

use strict;

sub sum
{
    my ($a, $b) = @_;

    return $a + $b;
}

my $s = sum(1, 2);
print("$s\n");

#----------------------------

sub sum2
{
    my ($first, @others) = @_;

    my $s = $first;

    foreach my $n (@others) {
        $s += $n;
    }

    return $s;
}

my @arr1 = (2, 3);

my $s = sum2(1, @arr1);
print("$s\n");

#----------------------------

sub sum3
{
    my ($first, $others) = @_;

    my @others = @$others;

    my $s = $first;

    foreach my $n (@others) {
        $s += $n;
    }

    return $s;
}

my @arr1 = (2, 3, 4);

my $s = sum3(1, \@arr1);
print("$s\n");


#----------------------------

sub sum4
{
    my ($first, $others) = @_;

    my %others = %$others;

    my $s = $first;

    foreach my $k (keys %others) {
        $s += %others{$k};
    }

    return $s;
}

my %hash = (two => 2, three => 3, four => 4, five => 5);
my $s = sum4(1, \%hash);
print("$s\n");

