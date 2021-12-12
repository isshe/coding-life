import nmap     # 需pip3 install python-nmap; brew install nmap;
import optparse

def nmap_scan(tgt_host, tgt_port):
    nm_scan = nmap.PortScanner()
    nm_scan.scan(tgt_host, tgt_port)

    state = nm_scan[tgt_host]['tcp'][int(tgt_port)]['state']
    print(" [*] " + tgt_host + " tcp/" + tgt_port + " " + state)


def main():
    parser = optparse.OptionParser("Usage%prog " + "-H <target host> -p <target port>")
    parser.add_option("-H", dest='tgt_host', type="string", help="specify target host")
    parser.add_option("-p", dest="tgt_port", type="string", help="specify target port[s] separated by comma")

    (options, _) = parser.parse_args()
    tgt_host = options.tgt_host
    tgt_ports = str(options.tgt_port).split(', ')
    if (tgt_host is None) | (tgt_ports[0] is None):
        print(parser.usage)
        exit(0)
    
    for tgt_port in tgt_ports:
        nmap_scan(tgt_host, tgt_port)


if __name__ == "__main__":
    main()