#!/bin/bash

FILES="$1/*"
for f in $FILES
do
    ./lzwencode $f > /dev/null
    ./lzwdecode $f.compressed.lzw > /dev/null
    diff $f $f.compressed.lzw.decompressed.lzw
    rm $f.compressed.lzw $f.compressed.lzw.decompressed.lzw 

done


