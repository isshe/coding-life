# coding=utf-8

import dpkt
import socket
import optparse
from geoip2 import database

global db

def ret_geo_str(ip):
    rec = db.city(ip)
    return rec.subdivisions[0].names["zh-CN"]


def print_pcap(pcap):
    for (_, buf) in pcap:
        try:
            eth = dpkt.ethernet.Ethernet(buf)
            ip = eth.data   # 这里是二进制数据
            src = socket.inet_ntoa(ip.src)
            dst = socket.inet_ntoa(ip.dst)
            print("[+] Src: " + src + " --> Dst: " + dst)
            try:
                src_city = ret_geo_str(src)
            except:
                src_city = "内网/未知"

            try:
                dst_city = ret_geo_str(dst)
            except:
                dst_city = "内网/未知"

            if src_city and dst_city:
                print("[-] 从: " + src_city + " --> 到: " + dst_city)
        except:
            pass


def main():
    parser = optparse.OptionParser("Usage%prog -p <pacp_file.pcap> -g <geo_db_file.mmdb>")
    parser.add_option('-p', dest="pcap_file", type="string", help="specify pcap filename")
    parser.add_option('-g', dest="geo_db", type="string", help="specify geo db filename")

    (options, _) = parser.parse_args()
    if options.pcap_file is None or options.geo_db is None:
        print(parser.usage)
        exit(0)

    db = database.Reader(options.geo_db)
    
    pcap_file = options.pcap_file
    f = open(pcap_file, "rb") # 读取二进制文件，记得用rb
    pcap = dpkt.pcap.Reader(f)
    print_pcap(pcap)

if __name__ == "__main__":
    main()

