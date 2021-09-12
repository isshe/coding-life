import optparse
import pexpect.pxssh as pxssh

botNet = []

class Client:
    def __init__(self, host, user, password):
        self.host = host
        self.user = user
        self.password = password
        self.connected = True
        self.session = self.connect()

    def connect(self):
        try:
            s = pxssh.pxssh()
            s.login(self.host, self.user, self.password)
            self.connected = True
            return s
        except: #Exception, e:
            #print(e)
            self.connected = False
            print("[-] Error Connecting")
        return None
    
    def send_command(self, cmd):
        if self.connected:
            self.session.sendline(cmd)
            self.session.prompt()
            return self.session.before
        return "not connected"

def botnet_command(command):
    for client in botNet:
        output = client.send_command(command)
        print("[*] Output from " + client.host)
        print("[+] " + output + '\n')
    
def add_client(host, user, password):
    client = Client(host, user, password)
    botNet.append(client)   # 全局变量

def main():
    add_client("10.10.10.110", "root", "root")
    add_client("10.10.10.120", "root", "root")
    add_client("10.10.10.130", "root", "root")
    botnet_command("uname -v")
    botnet_command("cat /etc/issue")


if __name__ == "__main__":
    main()