import os
import shutil
from datetime import datetime


class Article(object):
    def __init__(self, script_path, blog_post_path, article_info, common_info):
        self.script_path = script_path
        self.blog_post_path = blog_post_path
        self.article_info = article_info
        self.common_info = common_info
        self.skip_list = {
            "dst-dir": True,
            "dst-path": True,
            "src-dir": True,
            "src-path": True,
            "filename": True,
        }
        self.add_more_detail()

    def format_info(self):
        need_info = {}
        skip_list = self.skip_list

        info = self.article_info
        for k, v in info.items():
            if skip_list.get(k, False):
                continue

            need_info[k] = v

        info = self.common_info
        for k, v in info.items():
            if skip_list.get(k, False):
                continue

            if need_info[k]:
                if isinstance(need_info[k], list) \
                        and isinstance(v, list):
                    # merge to list
                    for lv in v:
                        need_info[k].append(lv)
            else:
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

            if k == "categories":
                res = "{}{} = {}\n".format(res, "tags", v.lower())

        return res

    def add_more_detail(self):
        info = self.article_info
        common = self.common_info

        src_path = "{}/../../{}".format(
            self.script_path,
            common.get("src-path", "")
        )
        src_dir = info.get("src-dir", None)
        if src_dir:
            # src is directory
            src_path = src_path + "/" + src_dir
            files = os.listdir(src_path)
            for file in files:
                time = os.path.getmtime(src_path + "/" + file)
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
            time = os.path.getmtime(src_path + "/" + file)
            date = info.get('date', None)
            if not date or date < time:
                info['date'] = time

        if isinstance(info['date'], float):
            ts = int(info['date'])
            info['date'] = datetime.utcfromtimestamp(ts) \
                .strftime('%Y-%m-%d %H:%M:%S')

        info['author'] = 'isshe'

    def convert(self):
        info = self.article_info
        common = self.common_info

        dst_dir = info.get("dst-dir", "")
        dst_path = "{}/{}/{}".format(
            self.blog_post_path,
            common.get("dst-path", ""),
            dst_dir
        )

        src_path = "{}/../../{}".format(
            self.script_path,
            common.get("src-path", "")
        )
        src_dir = info.get("src-dir", None)
        if src_dir:
            # src is directory
            src_path = src_path + "/" + src_dir
            shutil.copytree(src_path, dst_path)
            os.remove(dst_path + '/' + info['filename'])
        else:
            # src is single file, just prepare dst dir
            os.mkdir(dst_path)

        # output
        index_file = dst_path + "/index.md"
        with open(index_file, "w+") as f:
            f.write("+++\n")
            f.write(self.format_info())
            f.write("+++\n\n")

            content_file = "{}/{}".format(src_path, info['filename'])
            with open(content_file, "r") as cf:
                f.write(cf.read())

        return dst_path
