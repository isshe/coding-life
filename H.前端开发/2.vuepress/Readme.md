
# vuepress

## 1.基本操作
### 1.1 开始写作：
```bash
yarn docs:dev # 或者：npm run docs:dev
```

### 1.2 生成静态 HTML 文件：
```bash
yarn docs:build # 或者：npm run docs:build
```

## 2.目录结构
* /
    * package.json
    * docs/
        * README.md
        * .vuepress/
            * dist/: 生成的静态文件放在这里
            * config.js: 配置文件。生成的静态文件存在地点可以在这里改
            * public/: 公共文件，如 favicons 和 PWA 的图标
            