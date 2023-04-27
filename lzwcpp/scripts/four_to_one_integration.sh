#!/bin/bash

FILES="$1/*"
for f in $FILES
do
	echo $f
    ./four_to_one_encode $f > /dev/null
    ./four_to_one_decode $f.compressed.4t1 > /dev/null
    diff -q $f $f.compressed.4t1.decompressed.4t1
    rm $f*compressed*

done


