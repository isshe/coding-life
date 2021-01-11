#!/bin/bash

projects=(
    "/root/code/Personal/Notebook"
    "/root/code/Personal/coding-life"
    "/root/code/Personal/Work"
    "/root/code/Personal/delivery-oredge"
)

for project in ${projects[@]}
do
    echo "---- project: $project start -----"
    cd $project
    no_changed=`git status | grep 'nothing to commit'`
    # echo "$no_changed"
    if [[ "x$no_changed" == "x" ]]; then
        branch=`git branch | grep '* ' | awk -F ' ' '{print $2}'`
        echo "[+] git pull: $branch"
        if [ $branch ]; then
            git pull origin $branch
        else
            git pull
        fi

        git add .
        now=`date +%Y%m%d`
        git commit -m "$now: daily sync."

        echo "[+] git push: $branch"
        if [ $branch ]; then
            git push origin $branch
        else
            git push
        fi
    fi
    echo "---- project: $project finished -----"
done

