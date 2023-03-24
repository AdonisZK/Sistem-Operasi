#!/bin/bash

cd "$(dirname "$0")"
hour=$(date +"%-H")
path=$(pwd)
filename=$(date +"%H:%M %d:%m:%y")
fullpath="$path/$filename"

encrypt() {
	msg=$1
	shift=$2
	
	lowercase=abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz
	uppercase=ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ
	encrypted=$(echo $msg | tr "${lowercase:0:26}" "${lowercase:${shift}:26}" | tr "${uppercase:0:26}" "${uppercase:${shift}:26}")
	
	echo $encrypted
}

cat /var/log/syslog | while read line 
do
	encrypt "$line" $hour >> "$fullpath".txt
done
	
# Konfigurasi cron
# 0 */2 * * * /path/to/log_encrypt.sh
# 0 */2 * * * /home/aurelioklv/sisop/praktikum/modul-1/soal4/log_encrypt.sh
