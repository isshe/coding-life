import sqlite3

def print_download(download_db):
    "读downloads.sqlite，但是Linux Firefox 70.0没有这个文件了"
    conn = sqlite3.connect(download_db)
    c = conn.cursor()
    c.execute("SELECT name, source, datetime(endTime/1000000, 'unixepoch') FROM moz_downloads")
    print("[*] ---file download---")
    for row in c:
        print("[+] File: " + str(row[0]), + "from source: " + str(row[1]) + " at: ", str(row[3]))

def print_cookies(db):
    try:
        conn = sqlite3.connect(db)
        c = conn.cursor()
        c.execute("select host, name, value FROM moz_cookies")
        print("\n[*] --- Found Cookies ---")
        for row in c:
            host = str(row[0])
            name = str(row[1])
            value = str(row[2])
            print("[+] Host: " + host + ", Cookies: " + name + ", Value: " + value)
    except Exception as e:
        print("error: " + str(e))


def print_history(db):
    try:
        conn = sqlite3.connect(db)
        c = conn.cursor()
        c.execute("select url, datetime(visit_date/1000000, 'unixepoch') \
            FROM moz_places, moz_historyvisits \
            where visit_count > 0 and moz_places.id==moz_historyvisits.place_id;")
        print("\n[*] --- Found History ---")
        for row in c:
            url = str(row[0])
            date = str(row[1])
            print("[+] " + date + ' - Visited: ' + url)
    except Exception as e:
        print("error: " + str(e))


def main():
    #print_download("~/.mozilla/firefox/wf3imn2k.default/")
    print_cookies("/home/isshe/.mozilla/firefox/wf3imn2k.default/cookies.sqlite")
    print_history("/home/isshe/.mozilla/firefox/wf3imn2k.default/places.sqlite")

if __name__ == "__main__":
    main()