# from pygal.i18n import COUNTRIES # 废弃了
from pygal.maps.world import COUNTRIES

for country_code in sorted(COUNTRIES.keys()):
    print(country_code, COUNTRIES[country_code])

