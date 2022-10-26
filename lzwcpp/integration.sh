./lzwencode testfiles/wap.txt > /dev/null
./lzwdecode testfiles/wap.txt.compressed.lzw > /dev/null
diff testfiles/wap.txt testfiles/wap.txt.compressed.lzw.decompressed.lzw
rm testfiles/wap.txt.compressed.lzw testfiles/wap.txt.compressed.lzw.decompressed.lzw 