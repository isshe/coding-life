from scapy.all import *

probe_reqs = []

def sniff_probe(pkt):
    if pkt.haslayer(Dot11ProbeReq):
        net_name = pkt.getlayer(Dot11ProbeReq).info
        if net_name not in probe_reqs:
            probe_reqs.append(net_name)
            print("[+] Detected New Probe Request: " + net_name)


def main():
    interface = "mon0" # "wlp5s0" #"wlx085700a16f17" #
    sniff(iface=interface, prn=sniff_probe)

if __name__ == "__main__":
    main()
