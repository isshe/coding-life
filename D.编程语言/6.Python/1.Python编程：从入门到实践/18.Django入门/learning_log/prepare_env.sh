#!/bin/bash


sudo pip3 install virtualenv

virtualenv ll_env               # 新建虚拟环境newenv

source ll_env/bin/activate      # 激活环境

pip install django

django-admin --version

deactivate                      #

django-admin startproject learning_log .