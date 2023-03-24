#!/bin/bash

function login {
	echo "-----Login-----"
    echo -n "Enter username: "
    read username
    echo -n "Enter password: "
    read -s password
    echo ""

# Mengecek kesusaian username dan password dari file /users/users.txt
    if ! grep -q "^$username:$password" /users/users.txt; then
        echo "LOGIN: ERROR Failed login attempt on user $username"
        echo "$(date +"%y/%m/%d %H:%M:%S") LOGIN: ERROR Failed login attempt on user $username" >> log.txt
    else
# Jika sesuai maka berhasil login
		echo "LOGIN: INFO User $username logged in"
		echo "$(date +"%y/%m/%d %H:%M:%S") LOGIN: INFO User $username logged in" >> log.txt
	fi
}
login
