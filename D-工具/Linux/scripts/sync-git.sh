#!/bin/bash

projects=(
    /root/code/Personal/Notebook
    # /root/code/Personal/coding-life
)

for project in $projects
do
    no_changed=`git status | grep 'nothing to commit'`
    if [ ! $no_changed ]; then
        branch=`git branch | grep '* ' | awk -F ' ' '{print $2}'`
        if [ $branch ]; then
            git pull origin $branch
        else
            git pull
        fi

        git add .
        git commit -m "change: daily sync."
        if [ $branch ]; then
            git push origin $branch
        else
            git push
        fi
    fi
done

