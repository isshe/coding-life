# 未测试：需在windows下测试
from _winreg import *

def val2addr(val):
    addr = ""
    for ch in val:
        addr += '%02x '% ord(ch)
        addr = addr.strip(" ").replace(" ", ":")[0:17]
    return addr


def print_nets():
    net = "SOFTWARE\Microsoft\Windows NT\CurrentVersion" + \
        "\NetworkList\Signatures\Unmanaged"
    key = OpenKey(HKEY_LOCAL_MACHINE, net)
    print("\n[*] Networks you have Joined.")
    for i in range(100):
        try:
            guid = EnumKey(key, i)
            net_key = OpenKey(key, str(guid))
            (n, addr, t) = EnumValue(net_key, 5)
            (n, name, t) = EnumValue(net_key, 4)
            mac_addr = val2addr(addr)
            net_name = str(name)
            print("[+] " + net_name + " " + mac_addr)
            CloseKey(net_key)
        except:
            break

def main():
    print_nets()

if __name__ == "__main__":
    main()