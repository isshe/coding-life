SystemTap 安装
---


```
# rpm
sudo yum install gcc gcc-c++ elfutils-devel

# deb
sudo apt-get install build-essential zlib1g-dev elfutils libdw-dev gettext

wget https://sourceware.org/systemtap/ftp/releases/systemtap-4.7.tar.gz
tar xf systemtap-4.7.tar.gz
cd systemtap-4.7/
./configure --prefix=/opt/stap --disable-docs \
            --disable-publican --disable-refdocs CFLAGS="-g -O2"
make -j2
sudo make install
```
