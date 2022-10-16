import time
from bluetooth import *

already_found = []

def find_bluetooth_devs():
    found_devs = discover_devices(lookup_names=True)
    for (addr, name) in found_devs:
        if addr not in already_found:
            print("[*] Found Bluetooth Device: " + str(name))
            print("[+] MAC address: " + str(addr))
            already_found.append(addr)

def rfcomm_con(addr, port):
    sock = BluetoothSocket(RFCOMM)
    try:
        sock.connect((addr, port))
        print("[+] RFCOMM addr " + addr + ", Port " + str(port) + ' open')
        sock.close()
    except Exception as e:
        print("[+] RFCOMM addr " + addr + ", Port " + " closed")

def main():
    while True:
        find_bluetooth_devs()
        for addr in already_found:
            for port in range(1, 30):
                rfcomm_con(addr, port)
        time.sleep(5)


if __name__ == "__main__":
    main()