
# Article json format

```json
{
    "categories": [
        "分类，一般中文，量少"
    ],
    "tags": [
        "标签，一般英文，量多"
    ],
    "src-path": "源路径",
    "dst-path": "目标路径",
    "articles": [
        {
            "title": "文章标题",
            "src-dir": "源文件夹，一般对于单 md 的文章省略",
            "dst-dir": "目标文件夹",
            "filename": "源文件名.md",
            "image": "文章的封面图片，一般不指定时，从资源库中随机选择一张",
            "tags": [
                "针对文章的额外标签"
            ],
            "date": "2020-10-15 15:14:32 +0800"
        }
    ]
}
```
