from anonBrowser import *
from bs4 import BeautifulSoup
import os
import optparse

def mirror_images(url, dir):
    ab = anonBrowser()
    ab.anonymize()
    html = ab.open(url)
    soup = BeautifulSoup(html)
    image_tags = soup.findAll('img')
    for image in image_tags:
        filename = image['src'].lstrip('http://')
        if not filename:
            continue
        filename = os.path.join(dir, filename.replace('/', '_'))
        print("[+] Saving " + str(filename) + ", src = " + image['src'])
        data = ab.open(image['src']).read()
        ab.back()
        save = open(filename, "wb")
        save.write(data)
        save.close()
    
def main():
    parser = optparse.OptionParser("Usage: prog -u <target url> -d <destination directory>")
    parser.add_option("-u", dest="tgt_url", type="string", help="specify target url")
    parser.add_option("-d", dest="dir", type="string", help="specify destination directory")
    (options, args) = parser.parse_args()

    url = options.tgt_url
    dir = options.dir
    if url is None or dir is None:
        print(parser.usage)
        exit(0)
    
    try:
        mirror_images(url, dir)
    except Exception as e:
        print("[-] Error Mirroring Images.")
        print("[-] " + str(e))

if __name__ == "__main__":
    main()