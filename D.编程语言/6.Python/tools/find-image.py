# coding=utf-8

import os
import shutil
from PIL import Image

# 定义目标文件夹路径
folder_path = "./src/"
dst_path = "./1920x1080"

# 定义要查找的分辨率
target_resolution = (1920, 1080)

if not os.path.exists(dst_path):
    os.makedirs(dst_path)

cur_dirs = list()
next_dirs = list()

cur_dirs.append(folder_path)
while len(cur_dirs) != 0:
    for path in cur_dirs:
        for root, dirs, files in os.walk(path):
            for file in files:
                # 判断是否为png或jpg图片
                if file.lower().endswith(('.png', '.jpg')):
                    img_path = os.path.join(root, file)
                    with Image.open(img_path) as img:
                        # 获取图片分辨率
                        width, height = img.size
                        # 如果分辨率符合，则输出图片路径
                        if (width, height) == target_resolution:
                            print(img_path)
                            shutil.copy(img_path, dst_path + '/' + file)

            for dir in dirs:
                next_dirs.append(os.path.join(root, dir))
            # next_dirs = next_dirs + dirs

    if len(next_dirs) != 0:
        cur_dirs = next_dirs
        next_dirs = list()
    else:
        cur_dirs = list()
        next_dirs = list()
