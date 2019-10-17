from django.contrib import admin

from learning_log.models import Topic, Entry

admin.site.register(Topic)
admin.site.register(Entry)