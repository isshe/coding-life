# google现在使用了ssl，不可以用这个脚本进行获取了。。。

import optparse
from scapy.all import *


def find_google(pkt):
    if pkt.haslayer(Raw):
        payload = pkt.getlayer(Raw).load
        #print(payload)
        if "GET" in payload:
            print("GET!!!")
            if  "google" in payload:
                r = re.findall(r"(?i)\&q=(.*?)\&", payload)
                if r :
                    search = r[0].split('&')[0]
                    search = search.replace('q=', '').replace('+', ' ').replace('%20', ' ')
                    print("[+] Searched For: " + search)

def main():
    parser = optparse.OptionParser("Usage: prog -i <interface>")
    parser.add_option("-i", dest="interface", type="string", help="specify interface to listen on")
    (options, _) = parser.parse_args()
    if options.interface == None:
        print(parser.usage)
        exit(0)
    conf.iface = options.interface

    try:
        print("[*] Starting Google Sniffer.")
        sniff(filter="tcp port 443", prn=find_google)
    except KeyboardInterrupt:
        exit(0)

if __name__ == "__main__":
    main()
