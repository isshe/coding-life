#!/usr/bin/perl

# perl cmd-opts.pl -b abc xxx
# perl cmd-opts.pl -h
# perl cmd-opts.pl -b abc -a aaa xxx


use v5.10.1;
use strict;
use warnings;

use Getopt::Std qw/ getopts /;
use vars qw/ $opt_a $opt_b /;

sub usage ($);

my %opts;
getopts 'ha:b:', \%opts or usage 1;

if ($opts{h}) {
    usage 0;
}

if ($opts{a}) {
    print("a = $opts{a}\n");
}

if ($opts{b}) {
    print("b = $opts{b}\n");
}

my $name = shift;
if (defined $name) {
    print("name = $name\n");
}

sub usage ($) {
    my $rc = shift;
    my $msg = <<_EOC_;
Usage:
    $0 [-h] [-a <a>] [-b <b>]
_EOC_
    if ($rc == 0) {
        print $msg;
        exit 0;
    }

    warn $msg;
    exit $rc;
}