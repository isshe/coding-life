#!/bin/bash
./main &

# https://github.com/brendangregg/FlameGraph/archive/v1.0.tar.gz
# not used  sudo yum -y install perl-CPAN

sudo python3 /usr/share/bcc/tools/profile -df -p `pgrep -n main` 5 > out.profile

kill -INT `pgrep -n main`

#./FlameGraph-1.0/flamegraph.pl < out.profile > out.svg
# Can't locate open.pm in @INC (you may need to install the open module) (@INC contains: /usr/local/lib64/perl5 /usr/local/share/perl5 /usr/lib64/perl5/vendor_perl /usr/share/perl5/vendor_perl /usr/lib64/perl5 /usr/share/perl5) at ./FlameGraph-1.0/flamegraph.pl line 97.
# BEGIN failed--compilation aborted at ./FlameGraph-1.0/flamegraph.pl line 97.

#sudo yum install perl-open.noarch
