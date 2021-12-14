#!/usr/bin/python3

import os
import sys
import getopt
import json
import shutil
from article import Article
from category import Category


def help():
    print(
        """
usage: python3 hblog.py -p [blog-path] [option]
Options:
-p      : specify the blog path.
        """
    )


def get_script_path():
    return os.path.dirname(os.path.realpath(__file__))


def main():
    script_path = get_script_path()

    blog_path = script_path + "/../../../blog"

    argv = sys.argv[1:]
    try:
        opts, _ = getopt.getopt(argv, "p:h")  # 短选项模式
    except:
        help()
        exit(1)

    for opt, arg in opts:
        if opt in ['-p']:
            if not os.path.exists(arg):
                print("Error: bad blog path")
                exit(1)
            blog_path = arg

    article_path = script_path + "/../articles"
    files = os.listdir(article_path)

    for file in files:
        # read file
        absfile = article_path + "/" + file
        with open(absfile) as f:
            data = json.load(f)
            if not data:
                print("Error: bad article file: ", file)
                exit(1)

        articles = data['articles']
        common_info = {}
        for k, v in data.items():
            if k == 'articles':
                continue

            common_info[k] = v

        articles_hash = {}
        for article in articles:
            articles_hash[article['dst-dir']] = True

        # NOTE: clean dst path
        blog_post_path = blog_path + "/content/post"
        dst_path = blog_post_path + '/' + common_info.get("dst-path", None)
        if not os.path.exists(dst_path):
            os.makedirs(dst_path)
        else:
            dirs = os.listdir(dst_path)
            for dir in dirs:
                if not articles_hash.get(dir, None):
                    print("[+] removing ", dst_path)
                    shutil.rmtree(dst_path)

        # process common category
        categories = common_info.get('categories', None)
        if isinstance(categories, list):
            for category in categories:
                ct = Category(blog_path, category)
                ct.add()

        for article in articles:
            # process category
            categories = article.get('categories', None)
            if isinstance(categories, list):
                for category in categories:
                    ct = Category(blog_path, category)
                    ct.add()

            # process article
            atc = Article(script_path, blog_path, article, common_info)
            atc.convert()


if __name__ == "__main__":
    exit(main())
