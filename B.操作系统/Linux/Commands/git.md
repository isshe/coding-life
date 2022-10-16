[TOC]

git
---

# submodule
## 添加submodule：
```
git submodule add <subproject URL>
> git submodule add https://github.com/isshe/gitsubproject
```

## 查看subproject的commit：
```
git diff --cached <subproject dir>
> git diff --cached gitsubproject/
```

## 查看subproject的.gitmodules：
```
git diff --cached --submodule
```

## 自动clone每个submodule：
```
git clone --recursive <your repository url>
> git clone --recursive https://github.com/isshe/gitproject
```

## 拉取子模块：
```
git submodule init
git submodule update
> 更常用的是：递归更新
git submodule update --init --recursive
```
子模块的后续更新：
```
git submodule update --remote
```

## 调整submodule的commit：
```
git pull origin xxx
```

## 切submodule的分支：
```
git config -f .gitmodules submodule.<submodule dir>.branch <submodule target branch name>
> git config -f .gitmodules submodule.DbConnector.branch stable
```
更新到相应的分支：
```
git checkout -b <branch name> --track origin/<branch name>
> git checkout -b chudai  --track origin/chudai
```

## 删除一个submodule：
* Delete the relevant section from the `.gitmodules` file.
* Stage the `.gitmodules` changes `git add .gitmodules`
* Delete the relevant section from `.git/config`.
* Run `git rm --cached path_to_submodule` (no trailing slash).
* Run `rm -rf .git/modules/path_to_submodule`
* Commit `git commit -m "Removed submodule <name>"`
* Delete the now untracked submodule files`rm -rf path_to_submodule`

# 使用示例
## 显示某次提交修改的文件
```bash
git show --name-only HEAD
```

# git merge squash
```bash
git checkout <your-dev-branch>
# -i 后跟 commit id，要合并的提交的前一个，如果你要合并两次提交，那个出来的编辑器里应该有两条记录
git rebase -i HEAD~2
# 编辑框出来后，可以把后面的提交的 pick 改为 squash
git pull --rebase origin master
git push -f origin <your-dev-branch>  #

git checkout master
git pull origin master
git merge <your-dev-branch>
git push origin master
```

# git tag

* 打 tag

```
git tag -a {tag name} {commit id}
```

* 推送 tag 到远端
```
git push origin {tag name}
```