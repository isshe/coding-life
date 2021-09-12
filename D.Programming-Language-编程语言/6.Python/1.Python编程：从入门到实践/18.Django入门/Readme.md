
Django
---

# Django工作流程
```
urls -> views(GET) -> .html -> views(POST)
```

# 虚拟环境
```
# sudo pip install virtualenv
pip3 install virtualenv
mkdir ~/newproject
cd ~/newproject
#virtualenv newenv               # 新建虚拟环境newenv
virtualenv ll_env --python=python3

source newenv/bin/activate      # 激活环境
pip3 install django
pip3 install Django
django-admin --version
django-admin startproject <project> .   # 新建项目
deactivate                      # 

# 安装django-bootstrap3
pip3 install django-bootstrap3

# 创建应用
python3 manage.py startapp learning_logs

# 修改数据库
python3 manage.py makemigrations learning_logs

# 创建/迁移数据库
python3 manage.py migrate

# 启动服务
python3 manage.py runserver

# python3 manage.py shell
```

# django项目
> 前置条件：激活环境

```python
# 新建项目
django-admin startproject <project> .

# 创建数据库
python manage.py migrate

# 
python manage.py runserver [端口]

# 
python manage.py makemigrations learning_log
```