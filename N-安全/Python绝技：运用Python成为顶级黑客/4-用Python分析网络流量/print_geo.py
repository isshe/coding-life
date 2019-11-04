
#  pip install python-geoip-geolite2
# pip install geoip2
from geoip2 import database

def print_record2(tgt):
    db = database.Reader("GeoLite2/GeoLite2-City.mmdb")
    rec = db.city(tgt)
    print(rec)


tgt = "173.255.226.98"
print_record2(tgt)