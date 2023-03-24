#!/bin/bash

target=$1
hour=$(echo ${target:0:2} | sed 's/^0*//')
path=$(pwd)
filename="$(basename -s .txt $target)-decrypted"
fullpath="$(echo "$path/$filename"|tr '\n' ' ')"

decrypt() {
	msg=$1
	shift=(26-$2)
	
	lowercase=abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz
	uppercase=ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ
	decrypted=$(echo $msg | tr "${lowercase:0:26}" "${lowercase:${shift}:26}" | tr "${uppercase:0:26}" "${uppercase:${shift}:26}")
	
	echo $decrypted
}

cat "$target" | while read line 
do
	decrypt "$line" $hour >> "$fullpath".txt
done
