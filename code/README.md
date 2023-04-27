Quick readme for this proj

## Bitio
bitio allows for reading + writiing bits
to run its unit tests
```
    make test_bitio
    ./bitio
```

## LZW
lzw is the main algoritm class
lzwencode and lzwdecode call that class

To encode
``` 
    make lzwencode
    ./lzwencode $FILE_NAME
``` 
to decode
```
    make lzwdecode
    ./lzwdecode $FILENAME
```

as of right now the encoded/decoded files will juse be in the same directory as the input.

to run unit tests
```
    make test_lzw
    ./test_lzw
```
to run integration tests
```
    make all
    sh integration.sh $FOLDER_NAME
```