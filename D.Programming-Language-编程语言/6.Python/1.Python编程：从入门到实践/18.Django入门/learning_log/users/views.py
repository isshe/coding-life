from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.urls import reverse
from django.contrib.auth import logout as django_logout
from django.contrib.auth import login as django_login
from django.contrib.auth import authenticate as django_authenticate
from django.contrib.auth.forms import UserCreationForm

# Create your views here.

def logout(request):
    """注销"""
    django_logout(request)
    return HttpResponseRedirect(reverse('learning_logs:index'))

def register(request):
    """注册新用户"""
    if request.method != "POST":
        form = UserCreationForm()
    else:
        form = UserCreationForm(data=request.POST)

        if form.is_valid():
            new_user = form.save()
            authenticated_user = django_authenticate(username=new_user.username,
                                              password=request.POST['password1'])
            django_login(request, authenticated_user)
            return HttpResponseRedirect(reverse('learning_logs:index'))

    context = {'form': form}
    return render(request, 'users/register.html', context)