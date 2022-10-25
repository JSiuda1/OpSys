#! usr/bin/bash

for var in "$@"
do
    if [ -f "$var" ]
    then
        echo "${var} -> Plik istnieje"
    elif [ -d "$var" ]
    then
        [ "$(ls -A "$var")" ] && echo "${var} -> Katalog" || echo "${var} -> Katalog pusty"
    else
        echo "${var} -> plik nie istnieje"
    fi
done