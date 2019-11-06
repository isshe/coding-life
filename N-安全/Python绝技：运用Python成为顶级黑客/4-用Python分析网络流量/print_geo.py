
#  pip install python-geoip-geolite2
# pip install geoip2
from geoip2 import database

def print_record2(tgt):
    db = database.Reader("GeoLite2/GeoLite2-City.mmdb")
    rec = db.city(tgt)
    
    print(rec)
    print(rec.country)
    print(rec.city)
    print(rec.subdivisions)
    print(rec.subdivisions[0].names["zh-CN"])


tgt = "14.17.22.45"
print_record2(tgt)