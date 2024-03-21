# C 语言

## snprintf

```
char buf[BUF_SIZE];
char *str = "...";

int exp_len = snprintf(buf, BUF_SIZE, "%s", str)
int real_len = BUF_SIZE > exp_len ? exp_len : BUF_SIZE - 1;
```

snprintf 的第二个参数是结果缓冲区的最大长度，限制只能写入的字符串长度；而返回值是想要写入的字符串长度，也就是 `"%s", str` 的长度。当 BUF_SIZE 大于 exp_len 时，返回的不带 `\0` 的字符串长度；否则，返回的是带 `\0` 的字符串长度。
