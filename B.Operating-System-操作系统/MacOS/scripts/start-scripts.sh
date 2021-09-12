#!/bin/bash

basepath=$(cd `dirname $0`; pwd)

chmod +x $basepath/*.sh

$basepath/isshe-change-network.sh &