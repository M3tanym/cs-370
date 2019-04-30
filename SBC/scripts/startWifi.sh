#!/bin/bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi
rfkill unblock wlan
create_ap -n --no-virt -g 192.168.42.1 wlan0 LattePanda CS370LattePanda &
