import json

from country_code import get_country_code


def v1():
    # 将数据加载到一个列表中
    filename = 'population_data.json'
    with open(filename) as f:
        pop_data = json.load(f)

    for pop_dict in pop_data:
        if pop_dict['Year'] == '2010':
            country_name = pop_dict['Country Name']
            population = int(float(pop_dict['Value']))
            code = get_country_code(country_name)
            if code:
                print(code + ': ' + str(population))
            else:
                print('ERROR - ' + country_name)


if __name__ == "__main__":
    v1()
