import os
import random
from utils import pick_color, pick_image
from shutil import copyfile
import shutil


class Category(object):
    def __init__(self, blog_path, category, desc=None):
        self.path = "{}/content/categories/{}".format(
            blog_path, category.replace(" ", "-"))
        self.name = category
        self.desc = desc
        self.image_path = blog_path + "/static"

    def add(self):
        category_path = self.path
        # TODO: fix this
        if os.path.exists(category_path):
            shutil.rmtree(category_path)

        print("[+] Adding category: ", self.name)
        os.mkdir(category_path)
        color = pick_color()
        image = pick_image(self.image_path)
        _, file_extension = os.path.splitext(image)
        new_image_name = "image" + file_extension
        copyfile(self.image_path + '/' + image,
                 category_path + '/' + new_image_name)
        index_path = category_path + '/_index.md'
        with open(index_path, 'w+') as f:
            f.write("---\n")
            f.write('title: "{}"\n'.format(self.name))
            f.write('slug: "{}"\n'.format(self.name))
            f.write('image: "{}"\n'.format(new_image_name))
            f.write('style:\n')
            f.write('   background: "{}"\n'.format(color))
            f.write('   color: "#fff"\n')
            f.write("---\n\n")
