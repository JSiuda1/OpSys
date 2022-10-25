#! usr/bin/bash

if [ "$#" != "3" ]; then
    echo "Program wymaga 3 argumentow"
    exit -1
fi

file_extension=$1
days=$2
archive_name=$3
#tar -> c - create, v - wypisz zapisane pliki f - umozliwa nadanie nazwy
#-name -> nazwa pliku
#-mtime -> czas od ostatniej modyfikacji
echo "Zapisano do archiwum: "
tar -cvf "$archive_name.tar" `find \. -perm -a=r -name "*$file_extension" -mtime "-$days"`