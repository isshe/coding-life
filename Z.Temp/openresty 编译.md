

```bash
apt install dos2unix mercurial openresty-yajl-dev

cd openresty

# 如需访问私有仓库
export GITHUB_API_TOKEN=xx

# make
./util/mirror-tarballs

cd openresty-1.19.9.1.16
./configure
make -j4
make install
```
