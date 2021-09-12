#!/bin/bash

projects=(
    "/root/persional/coding-life"
    "/root/persional/xway/xway-frontend"
    "/root/persional/xway/xway-backend"
    "/root/code/Personal/coding-life"
)

for project in "${projects[@]}"
do
    echo "---- project: $project start -----"
    cd "$project" || continue
    no_changed=$(git status | grep 'nothing to commit')
    # echo "$no_changed"
    if [ -z "$no_changed" ]; then
        branch=$(git branch | grep '* ' | awk -F ' ' '{print $2}')
        echo "[+] git pull: $branch"
        if [ -n "$branch" ]; then
            git pull origin $branch
        else
            git pull
        fi

        git add .
        now=$(date +%Y%m%d)

        comment="$now: daily sync."
        if git status -s | grep 'M '; then
            first_file=$(git status -s | grep 'M ' | awk -F ' ' '{print $2}' | head -n 1)
            comment="change: modified $first_file ($now daily sync)."
        fi

        if git status -s | grep 'D '; then
            first_file=$(git status -s | grep 'D ' | awk -F ' ' '{print $2}' | head -n 1)
            comment="change: deleted $first_file ($now daily sync)."
        fi

        if git status -s | grep '?? '; then
            first_file=$(git status -s | grep '?? ' | awk -F ' ' '{print $2}' | head -n 1)
            comment="change: added $first_file ($now daily sync)."
        fi

        git commit -m "$comment"

        echo "[+] git push: $branch"
        if [ -n "$branch" ]; then
            git push origin $branch
        else
            git push
        fi
    fi
    echo "---- project: $project finished -----"
done

