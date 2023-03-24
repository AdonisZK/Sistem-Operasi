#!/bin/bash

echo "--1a--"
awk -F',' '/Japan/ {print $2}' Praktikum1.csv | head -5

echo -e "\n--1b--"
LC_ALL=C sort -t',' -k9,9n Praktikum1.csv | awk -F',' '/Japan/ {print $2}' | head -5

echo -e "\n--1c--"
sort -t',' -k20,20n Praktikum1.csv | awk -F',' '/Japan/ {print $2}' | head -10

echo -e "\n--1d--"
awk -F',' '/Keren/ {print $2}' Praktikum1.csv
