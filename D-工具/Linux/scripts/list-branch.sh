#!/bin/bash

basepath=$(cd `dirname $0`; pwd)
workpath="$basepath/../orinc"

projects=( `ls ${workpath}` )

for project in ${projects[@]}
do
    path="$workpath/$project"
    if [ -d $path -a -d $path/.git ]; then
        cd $path
        branch=`git branch | grep '\* ' | awk -F ' ' '{print $2}'`
        echo -e "$project: \t$branch"
    fi
done