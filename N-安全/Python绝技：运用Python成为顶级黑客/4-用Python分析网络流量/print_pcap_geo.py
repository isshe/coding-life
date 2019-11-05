import dpkt
import socket
import pygeoip
import optparse

def ret_geo_str(ip):
    pass
    

def print_pcap(pcap):
    for (ta, buf) in pcap:
        try:
            eth = dpkt.ethernet.Ethernet(buf)
            ip = eth.data
            src = socket.inet_ntoa(ip.src)
            dst = socket.inet_ntoa(ip.dst)
            print("[+] Src: " + src + " --> Dst: " + dst)
            print("[+] Src: " + ret_geo_str(src) + " --> Dst: " + ret_geo_str(dst))
        except:
            pass

def main():
    parser = optparse.OptionParser("Usage%prog -p <pacp file>")
    parser.add_option('-p', dest="pcap_file", type="string", help="specify pcap filename")

    (options, _) = parser.parse_args()
    if options.pcap_file is None:
        print(parser.usage)
        exit(0)
    pcap_file = options.pcap_file
    f = open(pcap_file)
    pcap = dpkt.pcap.Reader(f)  # ！！！
    print_pcap(pcap)

if __name__ == "__main__":
    main()

