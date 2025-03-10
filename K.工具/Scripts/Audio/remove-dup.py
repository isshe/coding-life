"""
音频文件重复检测和删除工具

这个脚本用于检测和删除重复的音频文件，主要功能包括：
- 检测重复的音频文件名
- 文件名标准化处理
- 删除重复文件，保留一个副本

主要功能：
1. 将文件名中的空格去除
2. 将中文转换为简体
3. 将英文转换为大写
4. 检测处理后文件名相同的文件
5. 保留第一个文件，删除其他重复文件

使用方法：
将脚本放在需要检查的音频文件目录下直接运行

处理规则：
- 去除文件名中的所有空格
- 中文统一转换为简体中文
- 英文字母统一转换为大写
- 重复文件中保留第一个，删除其他文件

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
            remove=False
            for file in files:
                print(f"  - {file}")
                src = os.path.join(current_dir, file)
                if remove:
                    os.remove(src)
                    print(f"已删除 {file}")
                else:
                    remove = True

if __name__ == "__main__":
    main()

