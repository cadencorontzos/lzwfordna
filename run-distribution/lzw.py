
# +++++++++ LZW Compression +++++++++
# Implementation by Caden Corontzos, Oct. 2021
# See Lisence for terms of use

import sys
import time
import random
import os
import math
sys.path.append(os.getcwd() + '/..')
from tidydata import TidyData

"""
    COMPRESS AND DECOMPRESS: both funtions take the name of the file they
    are meant to operate on, and the name of the file they are meant to 
    write to. The initial dictionary should vary by type of data. Generally,
    it should have at least the base characters of the file type.
    See: Possible Extensions (in README)
"""

def compress(filename, directoryName,  newFileName, td):
    codebook    = {chr(i-1).encode():i for i in range(1,257)}

    thefile = os.path.join(directoryName, filename)
    file        = open(thefile, 'rb')
    newFile     = open(newFileName, 'wb')
    codeword    = 257
    
    nextChar = file.read(1)
    currentBlock = ''.encode()
    while nextChar:  
        if (currentBlock+nextChar) in codebook:
            currentBlock+=nextChar
        else:
            td.start_record()

            td.add("File Name", "string", filename, "The name of the file")
            td.add("Corpus Name", "string", directoryName, "The name of the corpus")
            td.add("Run Length", "int", len(currentBlock), "Length of one run in the file")
            possibleCodeLength = math.ceil(math.log(codeword,2)/8)
            if currentBlock:
                newFile.write(codebook[currentBlock].to_bytes(possibleCodeLength, 'big'))
            else:
                newFile.write((0).to_bytes(possibleCodeLength, 'big'))
            newFile.write(nextChar)
            codebook[currentBlock+nextChar] = codeword
            codeword+=1
            currentBlock = ''.encode()
        nextChar = file.read(1)
    
    #if there was a character left over
    if currentBlock != ''.encode():
        newFile.write(codebook[currentBlock].to_bytes(possibleCodeLength, 'big'))
    file.close()
    newFile.close()

def decompress(compressedFileName, newFileName):
    codebook    = {i:chr(i-1).encode() for i in range(1,257)}
    file        = open(compressedFileName, 'rb')
    newFile     = open(newFileName, 'wb')
    codeword    = 257
    
    nextChar = file.read(1)
    currentBlock = ''.encode()
    while nextChar:  

        possibleCodeLength = math.ceil(math.log(codeword,2)/8)

        if len(currentBlock) < possibleCodeLength:
            currentBlock+=nextChar
        else:
            index = int.from_bytes(currentBlock,'big')
            if index != 0:
                theByte = codebook[index]
            else:
                theByte = ''.encode()
            theAdditionalByte = nextChar
            newFile.write(theByte)
            newFile.write(theAdditionalByte)
            codebook[codeword] = theByte+nextChar
            codeword+=1
            currentBlock = ''.encode()
        nextChar = file.read(1)
    
    #if there was a character left over
    if currentBlock:
            index = int.from_bytes(currentBlock,'big')
            theByte = codebook[index]
            newFile.write(theByte)

    file.close()
    newFile.close()

"""
    HELPER METHODS: These methods make for a clean report generation

"""

def printHeader(what):
    print('------------------------------------------------------------------------------')
    print(what.upper()+':')

def reportSize(filename, which):
    size = os.path.getsize(filename)
    print('    Size  : The size of the ' + which + ' file is '+ str(size) + ' bytes.')
    return size

def reportTime(start, end, what):
    totalTime = end - start
    print('    Time  : The time of ' + what + ' was ' + str(totalTime) + ' seconds.')

def reportCompressionRate(originalFileName, compressedFileName):
    original = os.path.getsize(originalFileName)
    compressed = os.path.getsize(compressedFileName)
    rate = original/compressed
    print('    Rate  : The compression rate was ' + str(rate) + '.')

def makeFilename(which, filename):
    ext = ''
    if '/' in filename:
        filename = filename.split('/')[1]
    if '.' in filename:
        ext = '.'+filename.split('.')[1]
        filename = filename.split('.')[0]
    if which =='compressed':
        ext = '.lzw'
    return 'outputFiles/' +which +'-'+filename+'-'+str(randSuffix) + ext

def assertCorrectness(originalFile, decompressedFile):
    oFileData = open(originalFile, 'rb')
    oFile = oFileData.read()
    dFileData = open(decompressedFile,'rb')
    dFile = dFileData.read()
    if oFile==dFile:
        print('    Check : The originial file and the decompressed file are the same.')
    else:
        print('    Check : Decomp error: The originial file and the decompressed file are not the same.')
    
def printFooter(names):
    print('------------------------------------------------------------------------------')
    print('To see the three versions of the file, see ', end = '')
    print(*names, sep=', \n', end='.\n')


"""
    REPORT GENERATION: On command line call, the name of the file is taken in. 
    The file is compressed, then decompressed. The logistics of this process 
    are printed to the console, the compressed and decompressed files are output
    into the directory, and the correctness of the algorithm is checked.

"""

if __name__ == "__main__":

    newFileName = "foo"
    for directoryName in ["DNACorpus1", "DNACorpus2"]:
        td = TidyData("run-distribution-"+ directoryName, "Distribution of runs in the corpus")
        files = os.listdir(directoryName)
        print(directoryName)
        for filename in files:
            print(filename)
            compress(filename, directoryName, newFileName, td )
            os.remove(newFileName)
        td.save()

    sys.exit()
    randSuffix = random.randint(10000,20000)
    names = []
    filename = sys.argv[-1]
    printHeader(filename)


    #original
    printHeader('original')
    fileData = open(filename)
    original = reportSize(filename, 'original')
    originalFileName = makeFilename('original', filename)
    names.append(filename)

    #compression
    printHeader('compression')
    compressedFileName = makeFilename('compressed', filename)
    names.append(compressedFileName) 
    start = time.time()
    compress(filename, compressedFileName)
    end = time.time()
    reportTime(start, end, 'compression')
    reportSize(compressedFileName, 'compressed')
    reportCompressionRate(filename, compressedFileName)

    #decompression
    printHeader('decompression')
    decompressedFileName = makeFilename('decompressed', filename)
    start = time.time()
    decompress(compressedFileName, decompressedFileName)
    end = time.time()
    names.append(decompressedFileName)
    reportTime(start, end, 'decompresssion')
    reportSize(decompressedFileName, 'decompressed')
    assertCorrectness(filename,decompressedFileName)

    #tells user where to view the files generated
    printFooter(names)
