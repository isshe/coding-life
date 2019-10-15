import pygal
import json

from country_code import get_country_code


def americas():
    # wm = pygal.Worldmap()
    wm = pygal.maps.world.World()
    wm.title = "North, Central, and South America"

    wm.add('North America', ['ca', 'mx', 'us'])
    wm.add('Central America', ['bz', 'cr', 'gt', 'hn', 'ni', 'pa', 'sv'])
    wm.add('South America', ['ar', 'bo', 'br', 'cl', 'co', 'ec', 'gf',
                             'gy', 'pe', 'py', 'sr', 'uy', 've'])

    wm.render_to_file('americas_tmp.svg')


def china():
    wm = pygal.maps.world.World()
    wm.title = "China"

    wm.add('China', {'cn': 1400000000, 'hk': 8000000, 'tw': 23000000})

    wm.render_to_file('china_tmp.svg')


def load_world_data():

    filename = 'population_data.json'
    with open(filename) as f:
        pop_data = json.load(f)

    cc_populations = {}
    for pop_dict in pop_data:
        if pop_dict['Year'] == '2010':
            country_name = pop_dict['Country Name']
            population = int(float(pop_dict['Value']))
            code = get_country_code(country_name)
            if code:
                cc_populations[code] = population
            else:
                print('ERROR - ' + country_name)
    return cc_populations


def world():
    wm = pygal.maps.world.World()
    wm.title = "World"

    cc_populations = load_world_data()

    wm.add('2010', cc_populations)

    wm.render_to_file('world_tmp.svg')


if __name__ == "__main__":
    americas()
    china()
    world()
