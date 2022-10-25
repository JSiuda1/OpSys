#!/bin/bash
while [ 1 ]
do
    ls -RaCl ~ | wc > /dev/null
    sleep 1
done