# coding=utf-8

import os
from PIL import Image
import hashlib

# png to jpg and compress
def compress_image(imagePath, outPath, quality=60):
    print("{} -> {}".format(imagePath, outPath))
    with Image.open(imagePath) as im:
        if im.mode != "RGB":
            im = im.convert("RGB")
        im.save(outPath, "JPEG", quality=quality)

def resize_image(imagePath, outPath, maxSize=1080, quality=80):
    print("{} -> {}".format(imagePath, outPath))
    with Image.open(imagePath) as im:
        if im.mode != "RGB":
            im = im.convert("RGB")
        width, height = im.size
        if max(width, height) <= maxSize:
            im.save(outPath, "JPEG", quality=quality)
        else:
            rate = max(width, height) / maxSize
            im.thumbnail((int(width / rate), int(height / rate)))
            im.save(outPath, "JPEG", quality=quality)

def get_dst_filename(src_filename):
    md5obj = hashlib.md5()
    md5obj.update(src_filename.encode('utf-8'))
    hash = md5obj.hexdigest()
    return hash

folder_path = "./1920x1080"
compress_root = "./1920x1080-compressed-images"
resize_root = "./1080x720-resize-images"

if not os.path.exists(compress_root):
    os.makedirs(compress_root)

if not os.path.exists(resize_root):
    os.makedirs(resize_root)

for root, dirs, files in os.walk(folder_path):
    for file in files:
        src_path = os.path.join(root, file)
        dst_file = get_dst_filename(file)
        compress_path = compress_root + '/' + dst_file + '.jpg'
        resize_path = resize_root + '/' + dst_file + '.jpg'
        compress_image(src_path, compress_path)
        resize_image(compress_path, resize_path)
