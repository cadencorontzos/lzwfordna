#!/bin/bash

FILES="$1/*"
for f in $FILES
do
	echo $f
    ./lzwencode $f > /dev/null
    ./lzwdecode $f.compressed.lzw > /dev/null
    diff -q $f $f.compressed.lzw.decompressed.lzw
    rm $f.compressed.lzw $f.compressed.lzw.decompressed.lzw 

done


