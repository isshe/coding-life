
Django
---

# 虚拟环境
```
# sudo pip install virtualenv
sudo pip3 install virtualenv
mkdir ~/newproject
cd ~/newproject
virtualenv newenv               # 新建虚拟环境newenv
source newenv/bin/activate      # 激活环境
pip install django
django-admin --version
django-admin startproject <project> .   # 新建项目
deactivate                      # 

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