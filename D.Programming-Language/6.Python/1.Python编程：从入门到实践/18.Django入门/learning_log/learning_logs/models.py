# coding=utf-8
from django.db import models
from django.contrib.auth.models import User

# Create your models here.

class Topic(models.Model):
    """用户学习的主题"""
    text = models.CharField(max_length=200)
    date_added = models.DateTimeField(auto_now_add=True)
    owner = models.ForeignKey(User, on_delete=True)

    def __str__(self):
        """返回模型的字符串"""
        return self.text


class Entry(models.Model):
    """学到的有关某个主题上的知识"""
    topic = models.ForeignKey(Topic, on_delete=True)
    text = models.TextField()
    date_added = models.DateTimeField(auto_now_add=True)

    class Meta:
        verbose_name_plural = 'entries'

    def __str__(self):
        if len(str(self.text)) > 50:
            return self.text
        else:
            return self.text[:50] + "..."