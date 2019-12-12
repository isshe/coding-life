[TOC]

第五章 公钥密码
---

# 秘钥配送问题
* 解决方法
  * 事先共享秘钥
  * 密钥分配中心
  * Diffie-Hellman密钥交换
  * 公钥密码


# RSA
RSA是一种公钥密码算法。名字来源于三位作者的姓。
* Ron Rivest、Adi Shamir、Leonard Adleman

## RSA加密
```
密文 = 明文 ^ E mode N 
```
* 在RSA中，明文、密钥、密文都是数字。
* 密文等于明文的E次方对N取模）
* E = Encryption
* N = Number 
* `E`和`N`的组合就是`公钥`。

## RSA解密
```
明文 = 密文 ^ D mod N
```
* D = Decryption
* N = Number
* `D`和`N`的组合就是`私钥`。

## 求N
```
N = p x q
```
* `p`和`q`是质数，由伪随机数生成器生成。

## 求L
```
L = lcm(p - 1, q - 1)
```
* lcm：取最小公倍数

## 求E
```
1 < E < L
gcd(E, L) = 1
```
* gcd: 最大公约数
* E和L最大公约数是1（互质）

## 求D
```
1 < D < L
E x D mode L = 1
```

# 中间人攻击
* 攻击方式：
  * A和B用公钥密码通讯。
  * 中间人X对A冒充B，对B冒充A。
* 中间人攻击对所有公钥密码对是有效的。