#!/bin/bash

FILES="$1/*"
for f in $FILES
do
	echo $f
    ./three_stream_lzwencode $f > /dev/null
    ./three_stream_lzwdecode $f.compressed.lzw > /dev/null
    diff -q $f $f.compressed.lzw.decompressed.lzw
    rm $f*compressed*

done


