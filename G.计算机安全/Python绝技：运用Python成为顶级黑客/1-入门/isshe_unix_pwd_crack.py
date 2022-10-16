import crypt

# 从/etc/shadow中拷贝内容过来
PASS_FILE_NAME = "shadow.tmp"
DICT_FILE_NAME = "dictionary.tmp"


def test_pass(crypt_pass, salt):
    dict_file = open(DICT_FILE_NAME, 'r')
    for word in dict_file.readlines():
        word = word.strip('\n')
        crypt_word = crypt.crypt(word, salt)
        if crypt_word == crypt_pass:
            print("[+] Found Password: " + word + "\n")
            return

    print("[-] Password Not Found.\n")
    return


def main():
    with open(PASS_FILE_NAME, 'r') as pf:
        for line in pf.readlines():
            if not ":" in line:
                continue

            user = line.split(':')[0]
            crypt_pass = line.split(':')[1].strip(' ')
            if not "$" in crypt_pass:
                continue
            crypt_type = crypt_pass.split('$')[1]
            crypt_salt = crypt_pass.split('$')[2]
            print("[*] Cracking Password For: " + user)
            if crypt_type == "6":
                new_crypt_salt = "$" + crypt_type + "$" + crypt_salt
                test_pass(crypt_pass, new_crypt_salt)
            elif crypt_type == "0":
                pass
            else:
                pass


if __name__ == '__main__':
    main()
