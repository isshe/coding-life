import PyPDF2
import optparse

from PyPDF2 import PdfFileReader

def print_meta(filename):
    pdf_file = PdfFileReader(open(filename, 'rb'))
    doc_info = pdf_file.getDocumentInfo()
    print("[*] PDF MetaData For: " + str(filename))
    for meta_item in doc_info:
        print("[+] " + meta_item + ":" + doc_info[meta_item])

def main():
    parser = optparse.OptionParser("Usage %prog " + "-F <PDF filename>")
    parser.add_option("-F", dest="filename", type="string", help="speify PDF file name")
    (options, args) = parser.parse_args()
    filename = options.filename
    if filename is None:
        print(parser.usage)
        exit(0)
    else:
        print_meta(filename)

if __name__ == "__main__":
    main()