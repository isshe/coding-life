import mechanize
import http.cookiejar as cookielib

def print_cookies(url):
    browser = mechanize.Browser()
    cookie_jar = cookielib.LWPCookieJar()
    browser.set_cookiejar(cookie_jar)
    page = browser.open(url)
    for cookie in cookie_jar:
        print(cookie)

if __name__ == "__main__":
    url = "http://www.syngress.com"
    print_cookies(url)