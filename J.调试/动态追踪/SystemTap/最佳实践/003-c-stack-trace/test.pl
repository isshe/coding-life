use strict;
use warnings;

use Time::HiRes qw(usleep nanosleep);

sub d()
{
    my $i = 0;
    while(1) {
        sleep(0.1);
        $i = $i + 1;
    }
}

sub c()
{
    d();
}

sub b()
{
    c();
}

sub a()
{
    b();
}

a()
