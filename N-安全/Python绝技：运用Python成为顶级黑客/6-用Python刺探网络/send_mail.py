# 登录不了

import smtplib
from email.mime.text import MIMEText

def send_mail(user, pwd, to, subject, text):
    msg = MIMEText(text)
    msg['From'] = user
    msg['To'] = to
    msg['Subject'] = subject

    try:
        smtp_server = smtplib.SMTP('smtp.gmail.com', 587)
        print("[+] Connecting To Mail Server.")
        smtp_server.ehlo()
        print("[+] Starting Encrypted Session.")
        smtp_server.starttls()
        smtp_server.ehlo()
        print("[+] Logging Into Mail Server.")
        smtp_server.login(user, pwd)
        print("[+] Sending Mail.")
        smtp_server.sendmail(user, to, msg.as_string())
        smtp_server.close()
        print("[+] Mail Sent Successfully")
    except Exception as e:
        print("[-] Sending Mail Failed: " + str(e))

def main():
    user = "abc"
    pwd = "bce"
    to = "efg"
    send_mail(user, pwd, to, 'Re:Important', 'Test Message')

if __name__ == "__main__":
    main()