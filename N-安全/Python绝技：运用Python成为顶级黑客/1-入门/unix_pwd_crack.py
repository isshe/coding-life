import crypt


def test_pass(crypt_pass, salt):
    dict_file = open('dictionary.tmp', 'r')
    for word in dict_file.readlines():
        word = word.strip('\n')
        crypt_word = crypt.crypt(word, salt)
        if crypt_word == crypt_pass:
            print("[+] Found Password: " + word + "\n")
            return

    print("[-] Password Not Found.\n")
    return


def main():
    pass_file = open('passwords.tmp')
    for line in pass_file.readlines():
        if ":" in line:
            user = line.split(':')[0]
            crypt_pass = line.split(':')[1].strip(' ')
            print("[*] Cracking Password For: " + user)
            salt = crypt_pass[0:2]
            test_pass(crypt_pass, salt)


if __name__ == '__main__':
    main()
