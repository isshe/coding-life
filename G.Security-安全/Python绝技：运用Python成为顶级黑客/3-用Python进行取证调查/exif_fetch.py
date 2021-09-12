import urllib.request
import optparse
from urllib.parse import urlsplit
from os.path import basename
from bs4 import BeautifulSoup
from PIL import Image
from PIL.ExifTags import TAGS

def find_images(url):
    print("[+] Finding images on " + url)
    url_content = urllib.request.urlopen(url).read()
    soup = BeautifulSoup(url_content)
    img_tags = soup.findAll('img')
    return img_tags

def download_image(img_tag):
    try:
        print("[+] Downloading image...")
        img_src = img_tag['src']
        img_content = urllib.request.urlopen(img_src).read()
        img_filename = basename(urlsplit(img_src)[2])
        #img_file = open(img_filename, 'wb')
        #img_file.write(img_content)
        #img_file.close()
        return img_filename
    except:
        return ''


def test_for_exif(img_filename):
    try:
        exif_data = {}
        img_file = Image.open(img_filename)
        info = img_file._getexif()
        if info:
            for (tag, value) in info.items():
                decoded = TAGS.get(tag, tag)
                exif_data[decoded] = value
                exif_GPS = exif_data['GPSInfo']
                if exif_GPS:
                    print("[*] " + img_filename + " contains GPS MetaData")

    except:
        pass


def main():
    parser = optparse.OptionParser('Usage%proc ' + '-u <target url>')
    parser.add_option('-u', dest='url', type='string', help='specify url address')
    (options, args) = parser.parse_args()
    url = options.url
    if url is None:
        print(parser.usage)
        exit(0)
    else:
        img_tags = find_images(url)
        for img_tag in img_tags:
            img_filename = download_image(img_tag)
            test_for_exif(img_filename)


if __name__ == "__main__":
    main()