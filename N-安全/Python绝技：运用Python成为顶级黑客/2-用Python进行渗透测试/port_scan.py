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

def port_scan(tgt_host, tgt_ports):
    try:
        tgt_ip = gethostbyname(tgt_host)
    except:
        print("[-] Cannot resolve '%s': Unknown host"%tgt_host)
        return
    
    try:
        tgt_name = gethostbyaddr(tgt_ip)
        print("\n[+] Scan Results for: " + tgt_name[0])
    except:
        print("\n[+] Scan Results for: " + tgt_ip)

    setdefaulttimeout(1)
    for tgt_port in tgt_ports:
        t = Thread(target=conn_scan, args=(tgt_host, int(tgt_port)))
        t.start()

def main():
    parser = optparse.OptionParser("Usage%prog " + "-H <target host> -p <target port>")
    parser.add_option("-H", dest='tgt_host', type="string", help="specify target host")
    parser.add_option("-p", dest="tgt_port", type="string", help="specify target port[s] separated by comma")

    (options, _) = parser.parse_args()
    tgt_host = options.tgt_host
    tgt_ports = str(options.tgt_port).split(', ')
    if (tgt_host is None) | (tgt_ports[0] is None):
        print(parser.usage)
        exit(0)
    
    port_scan(tgt_host, tgt_ports)


if __name__ == "__main__":
    main()