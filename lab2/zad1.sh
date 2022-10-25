#! usr/bin/bash
day_of_week=$(date +"%u")

if [ $day_of_week -gt 5 ]
then
    echo Weekend!
else
    echo Dzien pracujacy :C
fi