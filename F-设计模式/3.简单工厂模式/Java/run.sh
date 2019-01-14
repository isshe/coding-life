#!/bin/sh


script_file_path=$(cd `dirname $0`; pwd)

cd $script_file_path
javac -d . *.java
java Java.Test
