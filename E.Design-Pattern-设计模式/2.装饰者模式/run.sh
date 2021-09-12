#!/bin/sh


script_file_path=$(cd `dirname $0`; pwd)
cpp_path=${script_file_path}/CPP
java_path=${script_file_path}/Java

echo "--------------------Java--------------------"
cd ${java_path}
javac -d . *.java
java Java.StarbuzzCoffee
echo "--------------------------------------------"
echo ""

echo "--------------------CPP---------------------"
cd ${cpp_path}
g++ *.cpp
./a.out
echo "--------------------------------------------"
echo ""


