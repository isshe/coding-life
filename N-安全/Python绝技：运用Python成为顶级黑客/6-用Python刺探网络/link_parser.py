from anonBrowser import *
from bs4 import BeautifulSoup
import os
import optparse
import re
import sys

def print_links(url):
    ab = anonBrowser()
    ab.anonymize()
    page = ab.open(url)
    html = page.read()
    try:
        print("[+] Printing Links From Regex.")
        link_finder = re.compile('href="(.*?)"')
        print("---isshe----1---")
        links = link_finder.findall(html) #str(html))
        print("---isshe----2---")

        for link in links:
            print(link)
    except Exception as e:
        print("Unexpected error:", e) #sys.exc_info()[0])
        exit(0)

    try:
        print("\n[+] Printing Links From BeautifulSoup.")
        soup = BeautifulSoup(str(html), features="html5lib")
        links = soup.findAll(name='a')
        
        for link in links:
            if link.has_key('href'):
                print(link["href"])
    except:
        print("Unexpected error:", sys.exc_info()[0])


def main():
    parser = optparse.OptionParser("Usage%prog -u <target url>")
    parser.add_option("-u", dest="tgt_url", type="string", help="specify target url")
    (options, args) = parser.parse_args()
    url = options.tgt_url
    if url is None:
        print(parser.usage)
        exit(0)
    print_links(url)


if __name__ == "__main__":
    main()