import optparse

from socket import *
from threading import *

screen_lock = Semaphore(value=1)    # 信号量初始化为1，互斥量

def conn_scan(tgt_host, tgt_port):
    try:
        conn_skt = socket(AF_INET, SOCK_STREAM)
        conn_skt.connect((tgt_host, tgt_port))
        conn_skt.send('ViolentPython\r\n'.encode())
        results = conn_skt.recv(1024)

        screen_lock.acquire()
        print("[+] %d/tcp open" % tgt_port)
        print("[+] " + str(results))
    except:
        screen_lock.acquire()
        print('[-] %d/tcp closed' % tgt_port)
    finally:
        screen_lock.release()
        conn_skt.close()

