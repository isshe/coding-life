# coding=utf-8

# python3不符合预期，用python2

import pexpect

PROMPT = ['# ', '>>> ', '> ', '\$ ', ' ~', 'password for']

def send_command(child, cmd):
    child.sendline(cmd)
    child.expect(PROMPT)
    print(child.before)

def connect(user, host, password):

    conn_str = 'ssh ' + user + '@' + host
    print(conn_str)
    child = pexpect.spawn(conn_str)

    ssh_newkey = "Are you sure you want to continue connecting"
    ret = child.expect([pexpect.TIMEOUT, ssh_newkey, '[P|p]assword:'])
    if ret is 0:
        print("[-] Error Connecting")
        return
    if ret is 1:
        child.sendline('yes')
        ret = child.expect([pexpect.TIMEOUT, '[P|p]assword'])
        if ret is 0:
            print("[-] Error Connecting")
            return

    child.sendline(password)
    child.expect(PROMPT)
    print(child.before)
    return child

def main():
    host = "192.168.95.128"
    user = "isshe"
    password = "chudai"

    child = connect(user, host, password)
    send_command(child, 'cat /home/isshe/Code/helloworld')
    send_command(child, 'sudo cat /etc/shadow | grep isshe')
    send_command(child, password)


if __name__ == "__main__":
    main()