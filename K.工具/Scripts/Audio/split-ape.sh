#!/bin/bash

# APE 文件分割工具
#
# 这个脚本用于将 APE/FLAC/WAV 音频文件按照 CUE 文件进行分割，主要功能包括：
# - 支持 APE/FLAC/WAV 格式音频文件
# - 自动查找对应的 CUE 文件
# - 将 CUE 文件从 GB2312 转换为 UTF-8 编码
# - 按照 CUE 信息分割音频文件
# - 将分割后的文件转换为 MP3 格式
#
# 使用方法：
# 将脚本放在包含 APE/FLAC/WAV 和对应 CUE 文件的目录下直接运行
#
# 处理规则：
# - 查找目录下所有的 APE/FLAC/WAV 文件
# - 查找同名的 CUE 文件
# - 将 CUE 文件转换为 UTF-8 编码
# - 使用 shnsplit 按照 CUE 信息分割音频文件
# - 将分割后的文件转换为 320kbps 的 MP3 格式
#
# 依赖工具：
# - shnsplit: 音频分割工具
# - lame: MP3 编码工具
# - iconv: 字符编码转换工具

find . -type f \( -iname "*.ape" -o -iname "*.flac" -o -iname "*.wav" \) | while read -r file; do
    dir=$(dirname "$file")
    base=$(basename "$file")
    ext="${base##*.}"
    name="${base%.*}"

    if [[ $ext == "ape" ]] || [[ $ext == "flac" ]] || [[ $ext == "wav" ]] ; then
        cue_file="${dir}/${name}.cue"
        new_cue_file="${dir}/${name}2.cue"
        if [[ -f "$cue_file" ]]; then
            echo "转码成 UTF-8"
            iconv -f gb2312 -t utf-8 "$cue_file" > "$new_cue_file"

            echo "处理 APE: $file 使用 CUE: $cue_file"

            # 使用 shnsplit 拆分 APE 文件
            shnsplit -f "$new_cue_file" -t "%t" -o "cust ext=mp3 lame -b 320 - %f" "$file"

            # 可选: 删除原始的 APE 和 CUE 文件
            # rm -f "$file" "$cue_file"
        else
            echo "警告: 没有找到对应的 CUE 文件: $cue_file"
        fi
    fi
done

