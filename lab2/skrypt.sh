PLIK="lista_plikow_c.txt"
touch $PLIK
echo "$1" > $PLIK
echo $PLIK
echo $(cat $PLIK)
cp $PLIK kopia.txt
