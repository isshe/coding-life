import mechanize

def view_page(url):
    browser = mechanize.Browser()
    page = browser.open(url)
    source_code = page.read()
    print(source_code)

def test_proxy(url, proxy):
    browser = mechanize.Browser()
    browser.set_proxies(proxy)
    page = browser.open(url)
    source_code = page.read()
    print(source_code)

def main():
    # view_page('https://www.qq.com/')

    url = "http://ip.taobao.com/ipSearch.html"
    hide_me_proxy = {'http': '216.155.139.115:3129'}
    test_proxy(url, hide_me_proxy)

if __name__ == "__main__":
    main()