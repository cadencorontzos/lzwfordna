./lzwencode testfiles/wap.txt 
./lzwdecode testfiles/wap.txt.compressed.lzw
diff testfiles/wap.txt testfiles/wap.txt.compressed.lzw.decompressed.lzw
rm testfiles/wap.txt.compressed.lzw testfiles/wap.txt.compressed.lzw.decompressed.lzw 