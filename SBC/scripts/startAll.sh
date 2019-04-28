#!/bin/bash
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi

# must be executed from this directory as root
./startWifi.sh
./startLeap.sh
../code/main &


