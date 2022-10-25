#! usr/bin/bash

#aby skrypt działał poprawnie trzeba ustawić serwer SMTP
#sudo apt-get install ssmtp
#ustawic maila i haslo w pliku /etc/ssmtp/ssmtp.conf

while IFS= read -r line;
do
    sendmail "$line" < mail.txt
done < adresy.txt