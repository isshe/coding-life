import os
import random
import subprocess
import re


def pick_color():
    red = random.randint(80, 230)
    green = random.randint(80, 230)
    blue = random.randint(80, 230)
    return '#%02x%02x%02x' % (red, green, blue)
    # return "#"+''.join([random.choice('ABCDEF0123456789') for i in range(6)])


def pick_image(path):
    files = os.listdir(path)
    return random.choice(files)


def get_git_file_modtime(path, filename):
    cmd = "cd {} && git log --pretty='format:%ci' {} | head -n 1 | tr -d '\n'".format(
        path, filename)
    result = subprocess.run(['/bin/bash', '-c', cmd], stdout=subprocess.PIPE)
    return result.stdout.decode("utf-8")


def get_git_file_create_time(path, filename):
    cmd = "cd {} && git log --pretty='format:%ci' {} | tail -n 1 | tr -d '\n'".format(
        path, filename)
    result = subprocess.run(['/bin/bash', '-c', cmd], stdout=subprocess.PIPE)
    return result.stdout.decode("utf-8")


def get_blog_file_date(file):
    if not os.path.exists(file):
        return None

    with open(file, "r") as f:
        for line in f:
            # date = "2021-12-13"
            m = re.search(r'date = "([\d-]+).*"', line)
            if m:
                return m.group(1)

    return None


def fix_markdown_file_path(line):
    m = re.search('(.*)\[(.*)\]\(\./(.*)\)(.*)', line)
    if m:
        line = '{}[{}]({}){}\n'.format(
            m.group(1), m.group(2), m.group(3), m.group(4))

    return line


if __name__ == "__main__":

    path = '/root/persional/coding-life/Y.Blog/scripts'
    filename = 'to-hugo.py'
    print(get_git_file_modtime(path, filename))
    print(get_git_file_create_time(path, filename))

    file = '/root/persional/blog/content/post/coding-life/network/protocol/arp/index.md'
    print(get_blog_file_date(file))

    line = '![典型进程地址空间1](./process_address_map.png)'
    print(fix_markdown_file_path(line))
