"""
音频文件名检查工具

这个脚本用于检查音频文件名的规范性，主要功能包括：
- 检测重复的音频文件名
- 文件名标准化处理
- 移动重复文件到单独目录

主要功能：
1. 将文件名中的空格去除
2. 将中文转换为简体
3. 将英文转换为大写
4. 检测处理后文件名相同的文件
5. 将重复文件移动到 duplicates 目录

使用方法：
将脚本放在需要检查的音频文件目录下直接运行

处理规则：
- 去除文件名中的所有空格
- 中文统一转换为简体中文
- 英文字母统一转换为大写
- 重复文件会被移动到 duplicates 子目录

依赖库：
- zhconv: 中文简繁体转换
- opencc: 中文简繁体转换
"""

import os
import re
import shutil
from zhconv import convert
from opencc import OpenCC

def process_filename(filename):
    # 去掉空格
    filename = filename.replace(' ', '')

    # 将中文转换为简体
    filename = convert(filename, 'zh-cn')

    # 将英文转换为大写
    filename = re.sub(r'[a-zA-Z]+', lambda m: m.group(0).upper(), filename)

    return filename

def main():
    # 获取当前目录
    current_dir = os.getcwd()

    # 创建目标文件夹
    target_dir = os.path.join(current_dir, 'duplicates')
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)

    # 获取当前目录下所有的 mp3 文件
    mp3_files = [f for f in os.listdir('.') if f.endswith('.mp3')]

    # 处理文件名并存储
    processed_names = {}
    for file in mp3_files:
        name, ext = os.path.splitext(file)
        processed_name = process_filename(name)
        if processed_name in processed_names:
            processed_names[processed_name].append(file)
        else:
            processed_names[processed_name] = [file]

    # 输出处理后名字相同的文件
    for processed_name, files in processed_names.items():
        if len(files) > 1:
            print(f"处理后名字相同的文件：{processed_name}")
            for file in files:
                print(f"  - {file}")
                src = os.path.join(current_dir, file)
                dst = os.path.join(target_dir, file)
                shutil.move(src, dst)
                print(f"已移动 {file} 到 {target_dir}")

if __name__ == "__main__":
    main()

