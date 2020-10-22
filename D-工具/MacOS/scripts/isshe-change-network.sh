#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

my_name=`basename $0`
wifi_home="Wi-Fi-Home"
wifi_office="Wi-Fi"
my_pid=$$
# echo $my_pid
# check if another me is already executing
is_exist=`ps -axo pid,ppid,command | grep "${my_name}" | grep -v grep | grep -v ${my_pid}`
if [ ! -z "$is_exist" ]; then
    echo "Another program is already executing."
    exit 0
fi

print_array()
{
    array=$1
    all=""
    for one in ${array[@]}; do
        all="$all $one"
    done
    echo "$all"
}

# 如果顺序不对，就交换
swap_if_not_expect()
{
    pre=$1
    post=$2

    # 开启以换行分割
    set -f              # turn off globbing
    IFS='
'                   # split at newlines only
    service_array_str=`networksetup -listnetworkserviceorder | grep -v 'Hardware Port' | grep -v 'is disabled' | grep '(' | awk -F ')' '{print $2}'`
    service_array=()
    i=0
    for service in $service_array_str
    do
        service=`eval echo "$service"`
        # service=`echo "${service// /\\ }"`
        service_array[$i]=$service
        ((i++));
    done

    pre_pos=0
    post_pos=0
    i=0
    for((i=0;i<${#service_array[@]};i++))
    do
        if [ "${service_array[i]}" = "${pre}" ]; then
            pre_pos=$i
        fi
        if [ "${service_array[i]}" == "${post}" ]; then
            post_pos=$i
        fi
    done

    # 关闭以换行分割
    unset IFS
    set +f

    if [ $pre_pos -gt $post_pos ]; then
        # printf '%s ' "${service_array[@]}"
        # echo ""
        service_array[$post_pos]=$pre
        service_array[$pre_pos]=$post
        new_order=`printf "'%s' " "${service_array[@]}"`
        echo "$new_order"
        bash -c "networksetup -ordernetworkservices $new_order"
    fi

}



while true
do
    # get my current location by ip
    router=`networksetup -getinfo ${wifi_home} | grep -v IPv6 | grep 'Router' | grep -v grep | awk -F ' ' '{print $2}'`
    if [ "$router" = "192.168.31.1" ]; then
        # home
        swap_if_not_expect $wifi_home $wifi_office
    fi

    if [ "$router" = "192.168.50.1" ]; then
        # office
        swap_if_not_expect $wifi_office $wifi_home
    fi

    sleep 10
done
