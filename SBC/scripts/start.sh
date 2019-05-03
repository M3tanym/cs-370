#!/bin/bash

# this script gets called at boot by /etc/rc.local
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi


cd /home/user/Desktop/
cd CS370-Term-Project/SBC/scripts

systemctl start leapd

sudo ./startAll.sh

exit 0
