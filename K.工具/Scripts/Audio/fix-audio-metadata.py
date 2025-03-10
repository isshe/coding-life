"""
音频文件处理工具

这个脚本用于处理和转换各种音频文件格式，主要功能包括：
- 音频文件格式转换 (转换为 MP3)
- 音频文件元数据 (标签) 读取和修改
- 文件名解析和规范化
- 支持批量处理目录下的音频文件

支持的音频格式：
- MP3
- WAV
- FLAC
- M4A
- OGG
- DSF
- APE

主要功能：
1. 将非 MP3 格式音频转换为 MP3 格式
2. 读取和修正音频文件的元数据信息 (标题、艺术家、专辑等)
3. 支持从文件名解析歌曲信息
4. 支持简繁体中文转换
5. 支持批量处理和文件移动

使用方法：
python check-mp3-details.py -i <输入目录> -o <输出目录> -a <艺术家文件> [选项]

使用示例：（将 /root/input 的文件转换成 mp3 格式并修正元数据信息，然后替换掉 /root/input 中的原始文件）
python check-mp3-details.py -i /root/input -o /root/tmp-output -a artists.txt -m

参数说明：
-i, --input: 输入目录路径
-o, --output: 输出目录路径
-a, --artists: 艺术家名单文件路径
-m, --move-to-input: 将处理后的文件移动到输入目录
-c, --check: 仅检查不修改文件
-t, --test: 测试模式

依赖库：
- mutagen: 音频元数据处理
- ffmpeg-python: 音频格式转换
- magic: 文件类型检测
- opencc: 中文简繁体转换
"""

import magic
import sys
import os
import mutagen
import shutil
import argparse
import chardet
from mutagen.id3 import ID3, TIT2, TALB, TPE1, TRCK, TYER, Encoding, TextFrame
import ffmpeg
from mutagen.wave import WAVE
from mutagen.flac import FLAC
from mutagen.mp4 import MP4
from mutagen.oggvorbis import OggVorbis
from mutagen.dsf import DSF
from mutagen.apev2 import APEv2File
from mutagen import File
from mutagen.wave import WAVE
from mutagen.flac import FLAC
from mutagen.mp4 import MP4
from mutagen.oggvorbis import OggVorbis
from mutagen.dsf import DSF
from mutagen.monkeysaudio import MonkeysAudio

import opencc

temp_mp3_dir = ".to-mp3"
supported_file_types = {
    'audio/mpeg',
    'audio/x-wav',
    'audio/flac',
    'audio/ogg',
    'audio/x-dsf',
    'audio/x-ape',
    'audio/x-m4a',
}

def build_artists(artists_array):
    artists = dict()
    converter_s2t = opencc.OpenCC("s2t")  # 简体转繁体
    converter_t2s = opencc.OpenCC("t2s")  # 繁体转简体

    for name in artists_array:
        normalized_names = set()

        # 原始名称
        normalized_names.add(name)

        # 小写版本
        lower_name = name.lower()
        if lower_name != name:
            normalized_names.add(lower_name)

        # 转换为简体中文
        simplified_name = converter_t2s.convert(name)
        if simplified_name != name:
            normalized_names.add(simplified_name)

        # 转换为繁体中文
        traditional_name = converter_s2t.convert(name)
        if traditional_name != name:
            normalized_names.add(traditional_name)

        # 统一存入 artists 字典
        for n in normalized_names:
            artists[n] = name

    return artists

def read_artists(file_path):
    artists = list()
    with open(file_path, "r", encoding="utf-8") as file:
        for line in file:
            artists.append(line.strip())

    return artists



def contains_artist(part, artists):
    for artist in part.replace("&", "_").split("_"):
        if artist.strip() in artists:
            return True
    return False

def split_song_name(filename, artists):
    # 去掉文件扩展名
    # filename = filename.rsplit(".", 1)[0]
    # filename = os.path.splitext(filename)[0]

    # 尝试使用 " - " 或 "-" 进行拆分
    parts = filename.split(" - ") if " - " in filename else filename.split("-")

    # 如果拆分后不是两部分，则返回 None
    if len(parts) != 2:
        return filename, filename

    part1, part2 = parts

    # 判断哪一部分是艺人名
    if contains_artist(part1, artists):
        return part1.strip(), part2.strip()
    elif contains_artist(part2, artists):
        return part2.strip(), part1.strip()

    # 如果都不在 artists 中，则返回 None
    return filename, filename

def decode_text(text, text_encoding):
    if text_encoding == Encoding.LATIN1:
        encodings = ['gb2312', 'big5', 'gbk', 'utf-8']
        for encoding in encodings:
            try:
                decoded = text.encode('latin1').decode(encoding)
                return decoded
            except UnicodeDecodeError:
                continue
        return text  # 如果所有编码都失败，返回原始文本
    elif text_encoding in (Encoding.UTF8, Encoding.UTF16, Encoding.UTF16BE):
        return text  # UTF16 编码的文本已经正确，直接返回
    else:
        return text  # 其他情况，直接返回原始文本

def convert_to_mp3(input_file, output_file):
    print("converting file to mp3 ...")
    # try:
    # 设置输入
    stream = ffmpeg.input(input_file)

    # 设置输出参数
    # ffmpeg -hide_banner -loglevel error -i "$file" -c:a libmp3lame -b:a 320k -vn -y "$output_file" -nostdin

    stream = ffmpeg.output(stream, output_file,
                            acodec='libmp3lame',   # 音频编码器
                            audio_bitrate='320k',  # 音频比特率
                            map_metadata=0,        # 保留元数据
                            id3v2_version='3',     # 使用 ID3v2.3 版本，更兼容
                            vn=None,               # 不包含视频
                            loglevel='error'       # 设置日志级别 .global_args("-map_metadata")
                        ).global_args(
                            "-map_metadata", "0"      # 强制全局元数据映射（关键修复）
                        )

    # 运行 FFmpeg 命令
    # ffmpeg.run(stream, overwrite_output=True, capture_stdout=True, capture_stderr=True)
    try:
        ffmpeg.run(stream, overwrite_output=True, capture_stdout=True, capture_stderr=True)
    except ffmpeg.Error as e:
        print('Stderr:', e.stderr.decode('utf8'))
        raise Exception(e)

def get_file_type(file_path):
    try:
        file_type = magic.from_file(file_path, mime=True)
        detailed_type = magic.from_file(file_path)
        return True, file_type, detailed_type
    except Exception as e:
        return False, str(e), str(e)

def need_replace(key, text):
    equal_skip = {
        "公众号：小草新剧社",
        "公众号：小草新剧社，所有内容仅供试听，严禁商用，谢谢合作！！",
        "???",
        "??"
    }  # 使用集合以提高查找速度
    if text in equal_skip:
        return True

    contain_skip = {"??"}
    if any(skip_text in text for skip_text in contain_skip):
        return True

    return False

def decode_audio_info(info, file, artists, want_decode_info):
    if info is None:
        info = dict()

    decode_info = {}
    text = info.get('text', dict())
    encodings = info.get('encodings', dict())
    filename = os.path.splitext(os.path.basename(file))[0]
    artist, song_name = split_song_name(filename, artists)

    song_info = {
        'title': song_name,
        'artist': artist,
        'album': filename,
    }

    for key in ("title", "artist", "album"):
        value = text.get(key, None)
        if not value or need_replace(key, value):
            decode_info[key] = song_info[key]
        else:
            encoding = encodings.get(key, None)
            # for text in value.text:
            # print(f"key: {key}, v: {value}, encoding: {encoding}")
            decoded_text = decode_text(value, encoding)
            if want_decode_info or decoded_text != value:
                decode_info[key] = decoded_text

    print(f"原始信息：{info}")
    if decode_info:
        print(f"更新信息：{decode_info}")

    return decode_info

def get_mp3_info(file_path):
    info = {}
    audio = ID3(file_path)
    # audio = mutagen.File(file_path)
    for key in ["TIT2", "TPE1", 'TALB']:
        value = audio.get(key, None)
        info[key] = value

    return info

def get_audio_info(file_path, file_type):
    info = {}

    key_mapping = {
        'TIT2': 'title',
        'TPE1': 'artist',
        'TALB': 'album'
    }
    encoding_mapping = {
        'TIT2': 'title_encoding',
        'TPE1': 'artist_encoding',
        'TALB': 'album_encoding'
    }

    try:
        if file_type == "audio/mpeg":
            audio = ID3(file_path)
            for key in ["TIT2", "TPE1", 'TALB']:
                value = audio.get(key, None)
                if value:
                    info[encoding_mapping[key]] = value.encoding
                    info[key_mapping[key]] = value[0]
        elif file_type == 'audio/x-wav':
            audio = WAVE(file_path)
            for key in ["TIT2", "TPE1", 'TALB']:
                value = audio.get(key, None)
                if value:
                    info[encoding_mapping[key]] = value.encoding
                    info[key_mapping[key]] = value[0]
        elif file_type == 'audio/flac':
            audio = FLAC(file_path)
            info = audio.tags
        elif file_type == 'audio/ogg':
            audio = OggVorbis(file_path)
            info = audio.tags
        elif file_type == 'audio/x-dsf':
            audio = DSF(file_path)
            for key in ["TIT2", "TPE1", 'TALB']:
                value = audio.tags.get(key, None)
                if value:
                    info[encoding_mapping[key]] = value.encoding
                    info[key_mapping[key]] = str(value)
        elif file_type == 'audio/x-ape':
            audio = MonkeysAudio(file_path)
            for key, value in audio.tags.items():
                info[key.lower()] = str(value)
        elif file_type == 'audio/x-m4a':
            audio = MP4(file_path)
            info = audio.tags
        else:
            print(f"Unsupported file type: {file_type}")
            return None

        # 标准化输出
        standard_info = {
            'text': {
                'title': info.get('title', [None])[0] if isinstance(info.get('title'), list) else info.get('title'),
                'artist': info.get('artist', [None])[0] if isinstance(info.get('artist'), list) else info.get('artist'),
                'album': info.get('album', [None])[0] if isinstance(info.get('album'), list) else info.get('album'),
            },
            'encodings': {
                'title': info.get('title_encoding', None),
                'artist': info.get('artist_encoding', None),
                'album': info.get('album_encoding', None),
            }
        }

        return standard_info

    except Exception as e:
        print(f"Error processing file {file_path}: {str(e)}")
        return None


def fix_mp3_encoding(file_path, output_path):
    try:
        audio = mutagen.File(file_path)
        if audio:
            for key in audio:
                if isinstance(audio[key], TextFrame):
                    try:
                        audio[key].encoding = Encoding.UTF8
                    except:
                        pass
            audio.save(output_path)
        else:
            shutil.copy2(file_path, output_path)
    except Exception as e:
        print(f"Error fixing encoding for {file_path}: {e}")

def set_mp3_info(file_path, output_path, info):
    if not info:
        return True

    # 确保目标目录存在
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    # 复制文件到目标位置
    shutil.copy2(file_path, output_path)

    # 现在打开复制后的文件
    audio = mutagen.File(output_path)

    title = info.get('title', None)
    artist = info.get('artist', None)
    album = info.get('album', None)
    if title:
        audio['TIT2'] = TIT2(Encoding.UTF8, text=title)

    if artist:
        audio['TPE1'] = TPE1(Encoding.UTF8, text=artist)

    if album:
        audio['TALB'] = TALB(Encoding.UTF8, text=album)

    audio.save(output_path)

def main(args):
    output_dir = os.path.abspath(args.output)
    # if os.path.exists(output_dir):
    #     # 如果存在，删除整个目录及其内容
    #     shutil.rmtree(output_dir)

    artists = build_artists(read_artists(args.artists))
    decode_count = 0
    convert_count = 0
    total_count = 0
    change_count = 0
    for root, dirs, files in os.walk(args.input):
        # 检查当前目录是否为输出目录或其子目录
        if os.path.abspath(root).startswith(output_dir) and not os.path.abspath(args.input).startswith(output_dir):
            # 如果是，从 dirs 列表中移除，这样就不会遍历这个目录
            dirs[:] = []
            continue

        for file in files:
            file_path = os.path.join(root, file)

            if file.startswith('.'):
                # print(f"跳过隐藏文件：{file}")
                # if file in {"._.DS_Store", ".DS_Store"}:
                #     os.remove(file_path)

                continue

            if not file.lower().endswith(("mp3","wav","flac","m4a","ogg","dsf","ape")):
                continue

            relative_path = os.path.relpath(file_path, args.input)

            ok, file_type, detailed = get_file_type(file_path)
            if not ok:
                raise Exception(f"Failed to get file type, file: {file}")

            if file_type not in supported_file_types:
                unsupported_file_path = os.path.join(args.output, "_unsupport_files_", relative_path)
                os.makedirs(os.path.dirname(unsupported_file_path), exist_ok=True)
                shutil.copy2(file_path, unsupported_file_path)
                continue

            print(f"file: {file_path}\nfile type: {file_type}\ndetailed type: {detailed}")
            decode_file_path = file_path
            new_relative_path = relative_path
            converted = False
            if not file_type == "audio/mpeg":
                convert_count = convert_count + 1
                converted = True

                if not args.check:
                    new_relative_path, _ = os.path.splitext(relative_path)
                    new_relative_path = new_relative_path + ".mp3"
                    # new_file_path = os.path.join(directory, new_relative_path)
                    non_mp3_path = os.path.join(args.output, temp_mp3_dir, new_relative_path)
                    os.makedirs(os.path.dirname(non_mp3_path), exist_ok=True)
                    # shutil.copy2(file_path, non_mp3_path)
                    convert_to_mp3(file_path, non_mp3_path)
                    decode_file_path = non_mp3_path
                    # ok, file_type, detailed = get_file_type(decode_file_path)
                    # if not ok:
                    #     raise Exception(f"Failed to get decode file type, file: {file}")

            # info = get_mp3_info(decode_file_path)
            # info = get_audio_info(decode_file_path, file_type)
            info = get_audio_info(file_path, file_type)
            decoded_info = decode_audio_info(info, decode_file_path, artists, converted)
            decoded = False
            if decoded_info:
                decode_count = decode_count + 1
                decoded = True

                if not args.check:
                    output_path = os.path.join(args.output, new_relative_path)
                    os.makedirs(os.path.dirname(output_path), exist_ok=True)
                    set_mp3_info(decode_file_path, output_path, decoded_info)

            if args.move_to_input:
                output_path = os.path.join(args.output, new_relative_path)
                if not decoded_info and decode_file_path != file_path:
                    output_path = decode_file_path

                if converted or decoded:
                    print(f"移动 {output_path} 到 {file_path}, {decode_file_path}")

                if (converted or decoded) and os.path.exists(output_path):
                    if not args.check:
                        base, ext = os.path.splitext(file_path)
                        new_file_path = file_path
                        if ext.lower() != ".mp3":
                            new_file_path = f"{base}.mp3"
                            os.remove(file_path)
                        shutil.move(output_path, new_file_path)

            if converted or decoded:
                change_count = change_count + 1

            total_count = total_count + 1

            # if args.check:
                # filename = os.path.splitext(os.path.basename(file))[0]
                # artist, title = split_song_name(filename, artists)
                # print(f"filename: {filename}, artist: {artist}, title: {title}")

            print("-" * 50)
    print(f"需要更新信息的文件数量：{decode_count}，需要转码的文件数量：{convert_count}，需要变更的文件数量：{change_count}，总文件数量：{total_count}")

    # 删除处理过程产出的临时文件
    non_mp3_path = os.path.join(args.output, temp_mp3_dir)
    if os.path.exists(non_mp3_path):
        # 如果存在，则删除文件夹及其所有内容
        try:
            shutil.rmtree(non_mp3_path)
            print(f"临时文件夹 '{non_mp3_path}' 已被删除")
        except Exception as e:
            print(f"删除临时文件夹 '{non_mp3_path}' 时出错：{str(e)}")


def test(args):
    output_dir = os.path.abspath(args.output)
    # if os.path.exists(output_dir):
    #     # 如果存在，删除整个目录及其内容
    #     shutil.rmtree(output_dir)

    artists = build_artists(read_artists(args.artists))
    for root, dirs, files in os.walk(args.input):
        # 检查当前目录是否为输出目录或其子目录
        if os.path.abspath(root).startswith(output_dir) and not os.path.abspath(args.input).startswith(output_dir):
            # 如果是，从 dirs 列表中移除，这样就不会遍历这个目录
            dirs[:] = []
            continue

        for file in files:
            if not file.lower().endswith(("mp3","wav","flac","m4a","ogg","dsf","ape")):
                continue

            filename = os.path.splitext(os.path.basename(file))[0]
            artist, title = split_song_name(filename, artists)
            print(f"filename: {filename}, artist: {artist}, title: {title}")


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="MP3 file analyzer and fixer")
    parser.add_argument("-i", "--input", required=True, help="Input directory path")
    parser.add_argument("-a", "--artists", required=True, help="Specify the path to the artists file")
    parser.add_argument("-o", "--output", required=True, help="Output directory for fixed MP3 files")
    parser.add_argument("-m", "--move-to-input", action="store_true", help="Replace input file with output file")
    parser.add_argument("-c", "--check", action="store_true", help="Check Only")
    parser.add_argument("-t", "--test", action="store_true", help="Test")
    args = parser.parse_args()

    # print(build_artists())
    if args.test:
        test(args)
    else:
        main(args)

