# -*- coding: utf-8 -*-

# 检查TTL
# sudo apt-get install python-scapy
# sudo apt-get instlal python-ipy
# pip install scapy
# pip install IPy

import time
import optparse
from scapy.all import *
from IPy import IP as IPTEST

ttl_values = {}
THRESH = 5

def check_ttl(ipsrc, ttl):
    if IPTEST(ipsrc).iptype() == "PRIVATE":
        print(ipsrc + " is private")
        return
    
    if not ttl_values.has_key(ipsrc):
        pkt = sr1(IP(dst=ipsrc) / ICMP(), retry=0, timeout=1, verbose=0)
        ttl_values[ipsrc] = pkt.ttl
    
    print(ipsrc + ": ttl = " + ttl + ", ttl_2 = " + ttl_values[ipsrc])
    if abs(int(ttl) - int(ttl_values[ipsrc])) > THRESH:
        print("\n[!] Detected posible Spoofed Packet From: " + ipsrc)
        print("[!] TTL: " + ttl + ", Actual TTL: " + str(ttl_values[ipsrc]))

def test_ttl(pkt):c
    try:
        if pkt.haslayer(IP):
            ipsrc = pkt.getlayer(IP).src
            ttl = str(pkt.ttl)
            check_ttl(ipsrc, ttl)
    except:
        pass

def main():
    parser = optparse.OptionParser("Usage: " + "-i <interface> -t <thresh>")
    parser.add_option("-i", dest="iface", type="string", help="specify network interface")
    parser.add_option("-t", dest="thresh", type="string", help="specify threshold count")

    (options, args) = parser.parse_args()
    if options.iface is None:
        conf.iface = "eth0"
    else:
        conf.iface = options.iface
    
    if options.thresh is None:
        THRESH = 5

    sniff(prn=test_ttl, store=0)

if __name__ == "__main__":
    main()
