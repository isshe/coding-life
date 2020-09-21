[TOC]


GPG
---


# GPG 秘钥生成
> https://docs.github.com/en/github/authenticating-to-github/generating-a-new-gpg-key
> https://fedoraproject.org/wiki/Creating_GPG_Keys/zh-cn

```bash
# 生成 key
gpg2 --full-gen-key

# 记录下生成的信息：
# pub   rsa3072 2020-09-07 [SC]
#      BA5915D9BC85B8E3CB3FF99E8AA4EDDC7C5326C3
#uid                      heshushen (isshe) <shushen@openresty.com>
#sub   rsa3072 2020-09-07 [E]


# 上传到服务器
gpg2 --keyserver hkp://ha.pool.sks-keyservers.net --send-key BA5915D9BC85B8E3CB3FF99E8AA4EDDC7C5326C3
# 或者不指定服务器：gpg2 --send-key BA5915D9BC85B8E3CB3FF99E8AA4EDDC7C5326C3
# 上传到了 hkps://hkps.pool.sks-keyservers.net

# 导出公钥私钥
gpg2 --export-secret-keys --armor shushen@openresty.com > heshushen.asc
gpg2 --export --armor shushen@openresty.com > heshushen-pubkey.asc
```