import urllib.parse

from anonBrowser import *


# 这个google请求已经不能用了(403)
def google(search_term):
    ab = anonBrowser()
    search_term = urllib.parse.quote_plus(search_term)
    response = ab.open('http://ajax.googleapis.com/ajax/services/search/web?v=1.0&q=' + search_term)
    print(response.read())

if __name__ == "__main__":
    google("isshe")