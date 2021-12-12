from anonBrowser import *

if __name__ == "__main__":
    user_agents = [('User-Agent', 'superSecretBroswer')]
    ab = anonBrowser(proxies=[], user_agents=user_agents)
    for attempt in range(1, 5):
        ab.anonymize()
        print('[*] Fetching page')
        response = ab.open("http://kittenwar.com")
        for cookie in ab.cookie_jar:
            print(cookie)