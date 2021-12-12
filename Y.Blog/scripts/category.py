import os
import random
from utils import pick_color, pick_image
from shutil import copyfile


class Category(object):
    def __init__(self, blog_path, category, desc=None):
        self.path = "{}/content/categories/{}".format(
            blog_path, category.replace(" ", "-"))
        self.name = category
        self.desc = desc
        self.image_path = blog_path + "/static"

    def add(self):
        category_path = self.path
        if not os.path.exists(category_path):
            print("[+] Adding category: ", self.name)
            os.mkdir(category_path)
            color = pick_color()
            image = pick_image(self.image_path)
            copyfile(self.image_path + '/' + image,
                     category_path + '/' + image)
            index_path = category_path + '/_index.md'
            with open(index_path, 'w+') as f:
                f.write("---\n")
                f.write('title: "{}"\n'.format(self.name))
                f.write('slug: "{}"\n'.format(self.name))
                f.write('image: "{}"\n'.format(image))
                f.write('style:\n')
                f.write('   background: "{}"\n'.format(color))
                f.write('   color: "#fff"\n')
                f.write("---\n\n")
