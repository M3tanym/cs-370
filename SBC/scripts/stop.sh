#!/bin/bash

# this script gets called at shutdown by  /etc/acpi/events/powerbtn (had to edit etc/systemd/logind.conf to ignore power button)
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root"
   exit 1
fi

cd /home/user/Desktop/

sudo -u user tmux send-keys -t auto C-C
sleep 1
sudo -u user tmux kill-session -t auto
cd CS370-Term-Project/SBC/scripts
sudo ./stopWifi.sh
systemctl start leapd
echo "done!"
shutdown now
exit 0
