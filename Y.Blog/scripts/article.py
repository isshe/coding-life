import os
import re
import shutil
from datetime import datetime
from utils import pick_image, \
    get_git_file_modtime, \
    get_blog_file_date, \
    fix_markdown_file_path


class Article(object):
    def __init__(self, script_path, blog_path, article_info, common_info):
        self.script_path = script_path
        self.post_path = blog_path + '/content/post'
        self.image_path = blog_path + "/static"
        self.article_info = article_info
        self.common_info = common_info
        self.skip_list = {
            "dst-dir": True,
            "dst-path": True,
            "src-dir": True,
            "src-path": True,
            "filename": True,
        }
        self.is_single_file = True
        self.set_dst_path()
        self.set_src_path()
        self.add_more_detail()

    def set_dst_path(self):
        common = self.common_info
        info = self.article_info

        dst_dir = info.get("dst-dir", "")
        dst_path = "{}/{}/{}".format(
            self.post_path,
            common.get("dst-path", ""),
            dst_dir
        )
        self.dst_dir = dst_dir
        self.dst_path = dst_path

    def set_src_path(self):
        common = self.common_info
        info = self.article_info

        src_path = "{}/../../{}".format(
            self.script_path,
            common.get("src-path", "")
        )

        src_dir = info.get("src-dir", None)
        if src_dir:
            # src is directory
            src_path = src_path + "/" + src_dir
            self.is_single_file = False
        else:
            self.is_single_file = True

        self.src_path = src_path
        self.src_dir = src_dir

    def format_info(self):
        need_info = {}
        skip_list = self.skip_list

        info = self.article_info
        for k, v in info.items():
            if skip_list.get(k, False):
                continue

            need_info[k] = v

        res = ""
        for k, v in need_info.items():
            if isinstance(v, list):
                if len(v) > 0:
                    v = '[ "' + '", "'.join(v) + '" ]'
                else:
                    # next one
                    continue
            else:
                v = '"' + str(v) + '"'
            res = "{}{} = {}\n".format(res, k, v)

            # if k == "tags":
            #     print("v = ", v)

        return res

    def add_more_detail(self):
        info = self.article_info
        common = self.common_info
        src_path = self.src_path
        src_dir = self.src_dir

        if not self.is_single_file:
            # src is directory
            files = os.listdir(src_path)
            for file in files:
                time = get_git_file_modtime(src_path, file)
                date = info.get('date', None)
                if not date or date < time:
                    info['date'] = time

                if (file.endswith('.png')
                    or file.endswith('.jpg')
                    or file.endswith('.jpeg')) \
                        and info.get('image', None) is None:
                    info['image'] = file
        else:
            file = info.get("filename", None)
            time = get_git_file_modtime(src_path, file)
            date = info.get('date', None)
            if not date or date < time:
                info['date'] = time

        if isinstance(info['date'], float):
            ts = int(info['date'])
            info['date'] = datetime.utcfromtimestamp(ts) \
                .strftime('%Y-%m-%d')

        self.dst_index_date = get_blog_file_date(
            self.dst_path + '/' + 'index.md')

        info['author'] = 'isshe'

        # merge categories
        common_categories = common.get('categories', [])
        article_categories = info.get('categories', [])
        for category in common_categories:
            article_categories.append(category)

        info['categories'] = article_categories

        # merge tags
        tags_hash = {}

        tags = info.get('tags', [])
        for tag in tags:
            # tag = tag.lower()
            if not tags_hash.get(tag, None):
                tags_hash[tag] = True

        categories = info.get('categories', [])
        for category in categories:
            category = category.lower()
            if not tags_hash.get(category, None):
                tags_hash[category] = True

        common_tags = common.get('tags', [])
        for tag in common_tags:
            # tag = tag.lower()
            if not tags_hash.get(tag, None):
                tags_hash[tag] = True

        info['tags'] = [k for k, _ in tags_hash.items()]
        # print(info['tags'])

    def convert(self):
        info = self.article_info
        common = self.common_info
        dst_dir = self.dst_dir
        dst_path = self.dst_path
        src_path = self.src_path
        src_dir = self.src_dir

        # print("self.dst_index_date = ", self.dst_index_date)
        # print("info['date'] = ", info['date'])
        if self.dst_index_date and info['date'] == self.dst_index_date:
            print("[!] {} already up to date".format(info['title']))
            return
        elif os.path.exists(dst_path):
            shutil.rmtree(dst_path)

        print("[+] Converting articles: ", info['title'])
        if not self.is_single_file:
            # src is directory
            shutil.copytree(src_path, dst_path)
            os.remove(dst_path + '/' + info['filename'])
        else:
            # src is single file, just prepare dst dir
            os.makedirs(dst_path)

        if not info.get('image', None):
            image = pick_image(self.image_path)
            _, file_extension = os.path.splitext(image)
            new_image_name = "image" + file_extension
            shutil.copyfile(self.image_path + '/' + image,
                            self.dst_path + '/' + new_image_name)
            info['image'] = new_image_name

        # output
        index_file = dst_path + "/index.md"
        with open(index_file, "w+") as f:
            f.write("+++\n")
            f.write(self.format_info())
            f.write("+++\n\n")

            content_file = "{}/{}".format(src_path, info['filename'])
            with open(content_file, "r") as cf:
                for line in cf:
                    if line.startswith('[TOC]'):
                        continue
                    line = fix_markdown_file_path(line)
                    f.write(line)

        return dst_path
