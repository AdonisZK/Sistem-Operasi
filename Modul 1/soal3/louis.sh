#!/bin/bash

function register {
	echo "-----Register-----"
	echo -n "Enter username: "
    read new_username
    echo -n "Enter password: "
    read -s new_password
    echo ""

# Membuat file log.txt jika tidak ada
	if [ ! -f log.txt ]; then
		touch log.txt
	fi
	
# Mengecek jika username sudah terdaftar di file /users/users.txt
	if grep -q "^$new_username:" /users/users.txt; then
        echo "REGISTER: ERROR User already exists"
        echo "$(date +"%y/%m/%d %H:%M:%S") REGISTER: ERROR User already exists" >> log.txt
# Mengecek jika password memenuhi ketentuan yang ditentukan 
	elif ! echo "$new_password" | grep -q '[A-Z]' || ! echo "$new_password" | grep -q '[a-z]' || ! echo "$new_password" | grep -q '[0-9]'; then
        echo "REGISTER: ERROR Password tidak memenuhi ketentuan"
        echo "$(date +"%y/%m/%d %H:%M:%S") REGISTER: ERROR Password memerlukan minimal 1 huruf besar, 1 huruf kecil, dan 1 angka" >> log.txt
# Mengecek jika password == chicken
    elif echo "$new_password" | grep -q 'chicken'; then
        echo "REGISTER: ERROR Password tidak boleh mengandung kata chicken"
        echo "$(date +"%y/%m/%d %H:%M:%S") REGISTER: ERROR Password tidak boleh mengandung kata chicken" >> log.txt
# Mengecek jika password == ernie
    elif echo "$new_password" | grep -q 'ernie'; then
        echo "REGISTER: ERROR Password tidak boleh mengandung kata ernie"
        echo "$(date +"%y/%m/%d %H:%M:%S") REGISTER: ERROR Password tidak boleh mengandung kata ernie" >> log.txt
# Mengecek jika password sama dengan username
    elif [ "$new_password" = "$new_username" ]; then
        echo "REGISTER: ERROR Password tidak boleh sama dengan username"
        echo "$(date +"%y/%m/%d %H:%M:%S") REGISTER: ERROR Password tidak boleh sama dengan username" >> log.txt
# Menambahkan username dan password baru ke file /users/users.txt		
    else 
		echo "$new_username:$new_password" >> /users/users.txt
        echo "REGISTER: INFO User $new_username registered successfully"
        echo "$(date +"%y/%m/%d %H:%M:%S") REGISTER: INFO User $new_username registered successfully" >> log.txt
	fi
}
register

