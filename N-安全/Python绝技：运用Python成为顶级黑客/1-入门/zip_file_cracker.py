# coding=utf-8
import zipfile
import optparse
import sys

from threading import Thread


def extract_file(zfile, password):
    try:
        zfile.extractall(pwd=password.encode("ascii"))
        print("[+] Found password " + password + "\n")
    except:
        #print("Unexpected error: ", sys.exc_info())
        pass


def main():
    parser = optparse.OptionParser("Usage%prog " + "-f <zipfile> -d <dictionary>")
    parser.add_option('-f', dest='zname', type='string', help='specify zip file')
    parser.add_option('-d', dest='dname', type='string', help='specify dictionary file')

    (options, args) = parser.parse_args()
    if options.zname is None or options.dname is None:
        print(parser.usage)
        exit(0)
    else:
        zname = options.zname
        dname = options.dname

        zfile = zipfile.ZipFile(zname)  # 示例
        #print(zfile.infolist())
        pass_file = open(dname)
        for line in pass_file.readlines():
            password = line.strip('\n')
            t = Thread(target=extract_file, args=(zfile, password))
            t.start()

        # 多线程，会出错            
        #zfile.close()


if __name__ == '__main__':
    main()
