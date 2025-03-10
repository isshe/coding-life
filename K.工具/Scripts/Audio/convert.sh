#!/bin/bash

# 音频文件格式转换工具
#
# 这个脚本用于将各种音频格式转换为 MP3 格式，主要功能包括：
# - 递归查找所有音频文件
# - 将所有音频文件移动到当前目录
# - 将 OGG/FLAC/WAV 等格式转换为 MP3
# - 优化 MP3 文件的兼容性
#
# 支持的音频格式：
# - MP3: 直接复制
# - OGG: 转换为 320kbps MP3
# - FLAC: 转换为 320kbps MP3，保留元数据
# - WAV: 转换为 320kbps MP3
# - M4A: 转换为 320kbps MP3
# - DSF: 转换为 320kbps MP3
# - APE: 转换为 320kbps MP3
#
# 使用方法：
# 将脚本放在需要处理的音频文件目录下直接运行
#
# 处理规则：
# - 递归查找所有支持的音频格式文件
# - 将找到的文件移动到当前目录
# - 转换非 MP3 格式为 320kbps CBR MP3
# - 转换完成后删除原始文件
#
# 依赖工具：
# - ffmpeg: 音频格式转换
# - flac: FLAC 文件处理

# 定义要查找的文件后缀
suffixes=("mp3" "wav" "flac" "m4a" "ogg" "dsf" "ape")

# 遍历所有指定的后缀
for suffix in "${suffixes[@]}"; do
    echo "正在查找 .$suffix 文件..."

    find . -type f -iname "*.$suffix" | while read -r file; do
        echo "移动 $suffix: $file"
        mv "$file" ./
    done
done

# 转换OGG到MP3（兼容性优先）
find . -type f -iname "*.ogg" | while read -r file; do
    output_file="${file%.*}.mp3"
    echo "转换 OGG: $file → $output_file"
    ffmpeg -hide_banner -loglevel error -i "$file" -c:a libmp3lame -b:a 320k -vn -y "$output_file" -nostdin
    if [ $? -eq 0 ]; then
        rm -f "$file"
    fi
done

find . -type f -iname "*.flac" | while read -r file; do
    if ! flac -t "$file" >/dev/null 2>&1; then
        flac -d "$file" -c 2>/dev/null | flac - -o "$file.2" >/dev/null 2>&1
        if [ $? -eq 0 ]; then
            mv "$file.2" "$file"
        fi
    fi

    output_file="${file%.*}.mp3"
    echo "转换 FLAC: $file → $output_file"
    # 保留元数据（如专辑、艺术家信息），使用固定码率 320k CBR
    ffmpeg -hide_banner -loglevel error -i "$file" \
        -c:a libmp3lame -b:a 320k -map_metadata 0 \
        -vn -y "$output_file" -nostdin
    # ffmpeg -loglevel error -i "$file" -b:a 320k -y "$output_file"
    if [ $? -eq 0 ]; then
        rm -f "$file"
    fi
done

find . -type f -iname "*.wav" | while read -r file; do
    output_file="${file%.*}.mp3"
    echo "转换 WAV: $file → $output_file"
    # 保留元数据（如果有），使用固定码率 320k CBR
    ffmpeg -hide_banner -loglevel error -i "$file" \
        -c:a libmp3lame -b:a 320k -map_metadata 0 \
        -vn -y "$output_file" -nostdin
    if [ $? -eq 0 ]; then
        rm -f "$file"
    fi
done

find . -type f -iname "*.dsf" | while read -r file; do
    output_file="${file%.*}.mp3"
    echo "转换 DSF: $file → $output_file"
    # 使用 ffmpeg 转换 DSF 到 MP3，保留元数据，使用固定码率 320k CBR
    ffmpeg -hide_banner -loglevel error -i "$file" \
        -c:a libmp3lame -b:a 320k -map_metadata 0 \
        -vn -y "$output_file" -nostdin
    if [ $? -eq 0 ]; then
        rm -f "$file"
    fi
done

find . -type f -iname "*.ape" | while read -r file; do
    output_file="${file%.*}.mp3"
    echo "转换 APE: $file → $output_file"
    # 使用 ffmpeg 转换 APE 到 MP3，保留元数据，使用固定码率 320k CBR
    ffmpeg -hide_banner -loglevel error -i "$file" \
        -c:a libmp3lame -b:a 320k -map_metadata 0 \
        -vn -y "$output_file" -nostdin
    if [ $? -eq 0 ]; then
        rm -f "$file"
    fi
done

find . -type f -iname "*.m4a" | while read -r file; do
    output_file="${file%.*}.mp3"
    echo "转换 M4A: $file → $output_file"
    # 使用 ffmpeg 转换 M4A 到 MP3，保留元数据，使用固定码率 320k CBR
    ffmpeg -hide_banner -loglevel error -i "$file" \
        -c:a libmp3lame -b:a 320k -map_metadata 0 \
        -vn -y "$output_file" -nostdin
    if [ $? -eq 0 ]; then
        rm -f "$file"
    fi
done

# 定义要查找的文件后缀
suffixes=("mp3" "MP3")

# 遍历所有指定的后缀
for suffix in "${suffixes[@]}"; do
    echo "正在查找 .$suffix 文件..."

    find . -type f -iname "*.$suffix" | while read -r file; do
        echo "检查并修复文件：$file"
        mp3val -f "$file" >/dev/null 2>&1
    done
done

echo "删除空文件夹"
find . -type d -empty -delete

echo "所有操作已完成。"

