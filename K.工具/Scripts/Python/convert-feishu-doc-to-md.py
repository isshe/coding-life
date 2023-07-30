# -*- coding: utf-8 -*-

# 用法：用浏览器打开飞书，登录后得到 cookie，把 cookie 放到此脚本参数中执行。
# 示例：
#  python3 convert-feishu-doc-to-md.py -u 'https://ry5hwpuf7b.feishu.cn/docx/abc' -c 'abc'

import json
import warnings
import os
import sys
import argparse
import requests
from bs4 import BeautifulSoup
from urllib.parse import urlparse

script_path = os.getcwd()

def parse_args():
    description = "Convert feishu doc to markdown"
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument("-u", "--url", dest="url", action="store", required=True,
                        help="specify the doc url")
    parser.add_argument("-c", "--cookie", dest="cookie", action="store", required=True,
                        help="specify the cookie")
    args = parser.parse_args()

    return args

def request(uri, cookie):
    method = "GET"
    body = None

    headers = dict()
    headers["cookie"] = cookie

    r = requests.request(method, uri, headers=headers, json=body,
                        timeout=60, verify=True)

    if r.status_code != 200:
        return sys.exit('response status is not 200: {}\nresponse body: {}'.format(r.status_code, r.text))

    return r.text

def doc_parser(data_json_str):
    texts = list()

    data = json.loads(data_json_str)
    data = data['data']
    # print(data)

    block_map = data.get('block_map', dict())
    block_sequence = data.get('block_sequence', list())
    next_cursors = data.get("next_cursors", list())

    # print(next_cursors)

    for block in block_sequence:
        # print(block)
        if block_map[block]['data']['type'] == 'text':
            for t in block_map[block]['data']['text']['initialAttributedTexts']['text']:
                # print(block_map[block]['data']['text']['initialAttributedTexts']['text'][t])
                texts.append(block_map[block]['data']['text']['initialAttributedTexts']['text'][t])

        elif block_map[block]['data']['type'].startswith("heading"):
            header = "#" * int(block_map[block]['data']['type'][-1])
            if header:
                texts.append(header + " " + block_map[block]['data']['text']['initialAttributedTexts']['text']['0'])

        elif block_map[block]['data']['type'] == 'image':
            # TODO support image
            pass
        else:
            # print(block_map[block]['data'])
            pass

    return {
        'texts': texts,
        'next_cursors': next_cursors,
    }

def main(args=None):
    args = parse_args()

    doc = request(args.url, args.cookie)

    soup = BeautifulSoup(doc, "html.parser")

    target_script=None
    for script in soup.find_all("script"):
        # print("script: ", script)
        script = script.string
        if script is not None and script.startswith("window.DATA"):
            target_script = script

    if target_script is None:
        sys.exit("[!] 无法找到文章内容！")

    start_pos = target_script.find('{"data"')
    if start_pos != -1:
        print(f"[+] 找到数据起始位置: {start_pos}.")
    else:
        sys.exit("[!] 无法找到数据起始位置")

    end_pos = target_script.find(') });')
    if end_pos != -1:
        print(f"[+] 找到数据结束位置: {end_pos}.")
    else:
        sys.exit("[!] 无法找到数据结束位置")

    data_json_str = target_script[start_pos:end_pos]
    # print(data_json_str)

    parse_res = doc_parser(data_json_str)
    texts = parse_res["texts"]
    next_cursors = parse_res["next_cursors"]

    parsed_url = urlparse(args.url)
    # print(parsed_url)

    mode = 7
    limit = 239
    id = parsed_url.path.split('/')[-1]

    cur_cursors = next_cursors
    next_cursors = list()
    while len(cur_cursors) > 0 or len(next_cursors) > 0:
        for cursor in cur_cursors:
            url = "{}://{}/space/api/docx/pages/client_vars?id={}&mode={}&limit={}&cursor={}".format(
                parsed_url.scheme, parsed_url.netloc, id, mode, limit, cursor)
            cursor_data = request(url, args.cookie)

            parse_res = doc_parser(cursor_data)
            texts += parse_res["texts"]
            next_cursors += parse_res["next_cursors"]

        cur_cursors = next_cursors
        next_cursors = list()

    res_file = "/tmp/result.md"
    with open(res_file, "w+") as f:
        f.write("\n".join(texts))

    print("Finished.")

if __name__ == "__main__":
    exit(main())
