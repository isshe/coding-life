import requests
import pygal
from pygal.style import LightColorizedStyle as LCS, LightenStyle as LS


def v1(repo_dicts):
    # 研究第一个仓库
    repo_dict = repo_dicts[0]
    for key in sorted(repo_dict.keys()):
        print(key)


def v2(repo_dicts):
    names, stars = [], []
    for repo_dict in repo_dicts:
        names.append(repo_dict['name'])
        stars.append(repo_dict['stargazers_count'])

    my_style = LS('#336699', base_style=LCS)

    my_config = pygal.Config()
    my_config.x_label_rotation = 45
    my_config.show_legend = False
    my_config.title_font_size = 24      # 图表标题字体大小
    my_config.label_font_size = 14      # 副标签字体大小
    my_config.major_label_font_size = 18 # 主标签字体大小
    my_config.truncate_label = 15       # 太长截断为15字符
    my_config.show_y_guides = False     # 隐藏水平线
    my_config.width = 1000              # 自定义宽度

    chart = pygal.Bar(my_config, style=my_style)
    chart.title = "Most-Starred Python Projects on Github"
    chart.x_labels = names

    chart.add('', stars)
    chart.render_to_file("python_repos_tmp.svg")


def v3(repo_dicts):
    names, plot_dicts = [], []
    for repo_dict in repo_dicts:
        plot_dict = {
            'value': repo_dict['stargazers_count'],
            'label': repo_dict['description'] or "None",
            'xlink': repo_dict['html_url'],
        }
        plot_dicts.append(plot_dict)
        names.append(repo_dict['name'])


    my_style = LS('#336699', base_style=LCS)

    my_config = pygal.Config()
    my_config.x_label_rotation = 45
    my_config.show_legend = False
    my_config.title_font_size = 24      # 图表标题字体大小
    my_config.label_font_size = 14      # 副标签字体大小
    my_config.major_label_font_size = 18 # 主标签字体大小
    my_config.truncate_label = 15       # 太长截断为15字符
    my_config.show_y_guides = False     # 隐藏水平线
    my_config.width = 1000              # 自定义宽度

    chart = pygal.Bar(my_config, style=my_style)
    chart.title = "Most-Starred Python Projects on Github"
    chart.x_labels = names

    chart.add('', plot_dicts)
    chart.render_to_file("python_repos_tmp.svg")


if __name__ == "__main__":
    url = "https://api.github.com/search/repositories?q=language:python&sort=stars"

    r = requests.get(url)
    print("Status code: ", r.status_code)

    response_dict = r.json()
    print("Total repositories:", response_dict['total_count'])

    # 探索有关仓库的信息
    repo_dicts = response_dict['items']
    print("Repositories returned:", len(repo_dicts))

    v3(repo_dicts)
