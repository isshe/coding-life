# 检测出隐藏网络，检测出以后，放到unhidden_nets中。

from scapy.all import *

hidden_nets = []
unhidden_nets = []

def sniff_dot11(p):
    if p.haslayer(Dot11ProbeReq):
        addr2 = p.getlayer(Dot11).addr2
        if (addr2 in hidden_nets) & (addr2 not in unhidden_nets):
            net_name = p.getlayer(Dot11ProbeReq).info
            print("[+] Detected Hidden SSID: " + net_name + " for MAC: " + addr2)
            unhidden_nets.append(addr2)
    if p.haslayer(Dot11Beacon):
        if p.getlayer(Dot11Beacon).info == "":
            addr2 = p.getlayer(Dot11).addr2
            if addr2 not in hidden_nets:
                print("[-] Detected Hidden SSID: " + " for MAC: " + addr2)
                hidden_nets.append(addr2)


def main():
    interface = "mon0" # "wlp5s0" #"wlx085700a16f17" #
    sniff(iface=interface, prn=sniff_dot11)

if __name__ == "__main__":
    main()
