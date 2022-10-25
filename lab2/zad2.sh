#! usr/bin/bash
input=$1
command="ls -1"
if [ "$input" == "-a" ]
then
    command="${command} $1"
fi

$command | wc -l