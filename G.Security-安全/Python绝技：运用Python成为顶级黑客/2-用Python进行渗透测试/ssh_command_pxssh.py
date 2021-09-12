# coding=utf-8
import pexpect.pxssh as pxssh

def send_command(s, cmd):
    s.sendline(cmd)
    s.prompt()
    print(s.before)

def connect(host, user, password):
    try:
        s = pxssh.pxssh()
        s.login(host, user, password)
        return s
    except:
        print("[-] Error Connecting")
        exit(0)

host = "192.168.95.128"
user = "isshe"
password = "chudai"
s = connect(host, user, password)
send_command(s, 'cat /home/isshe/Code/helloworld')
