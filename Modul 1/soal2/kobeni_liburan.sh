#!/bin/bash

# Fungsi Download
function download_images {
  # Mendapatkan jam dalam format 24 jam
  HOUR=$(date +"%H")
	
  # Kalau jam 0 berarti lakukan 1 download selain itu sesuai dengan angka jam
  if [[ $HOUR -eq 0 ]]; then
      COUNT=1
  else
      COUNT=$HOUR
  fi

  # Mencari urutan folder berikutnya (kumpulan_0, kumpulan_1, ..., kumpulan_n)
  COUNTFOLDER=1
  while [[ -d "kumpulan_$COUNTFOLDER" ]]; do
      (( COUNTFOLDER++ ))
  done

  # Membuat directory untuk menyimpan foto
  DIRECTORY="kumpulan_$COUNTFOLDER"
  mkdir "$DIRECTORY"

  # Download image dari Indonesia melalui website Unsplash
  for (( i=1; i<=$COUNT; i++ ))
  do
      FILENAME="perjalanan_$i.jpg"
      echo "Downloading $FILENAME to $DIRECTORY"
	  wget --referer="https://unsplash.com/s/photos/indonesia" -q https://source.unsplash.com/800x600/?indonesia -O "$DIRECTORY/$FILENAME"       
	done
}

# Fungsi Zip
function zip_files {
	# Mencari urutan folder berikutnya (devil_1, devil_2, ..., devil_n)
    COUNTZIP=1
    while [[ -f "devil_$COUNTZIP.zip" ]]; do
        (( COUNTZIP++ ))
    done

    # Zip semua folder menggunakan Wild Card "kumpulan_*"
    zip -r "devil_$COUNTZIP.zip" kumpulan_*

	# Menghapus semua menggunakan Wild Card
    rm -r kumpulan_*/
}

# User input [download/zip] untuk memudahkan proses cronjob
if [[ $1 == "download" ]]; then
  download_images
elif [[ $1 == "zip" ]]; then
  zip_files
else
  echo "[download|zip]"
fi

# Cron Job
# Melakukan fungsi download setiap 10 jam
# 1 */10 * * * /home/adonis/adonis/praktikum1/kobeni_liburan.sh download
# Melakukan fungsi zip setiap hari baru
# 0 0 * * * /home/adonis/adonis/praktikum1/kobeni_liburan.sh zip



