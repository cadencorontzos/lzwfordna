import os
import subprocess
import sys
sys.path.append(os.getcwd() + '/..')
from tidydata import TidyData

LZW_ENCODE = './lzwencode'
LZW_DECODE = './lzwdecode'
CORPUS_1 = 'DNACorpus1'
CORPUS_2 = 'DNACorpus2'
N = 5

def parseEncodeOutput(output):
    output = output.split('\n')
    output = [ele.split(" : ") for ele in output]
    
    return output[0], output[1], output[2], output[3]

def parseDecodeOutput(output):
    output = output.split('\n')
    output = [ele.split(" : ") for ele in output]

    return output[0], output[1]

def runFile(filename, td):

    totalCompTime = 0
    totalDecompTime = 0
    for i in range(N):
        compressionOutput = subprocess.run([LZW_ENCODE, filename], capture_output=True)
        decompressionOutput = subprocess.run([LZW_DECODE, filename + ".compressed.lzw"], capture_output=True)
        decompressedFileSize, decompressionTime = parseDecodeOutput(decompressionOutput.stdout.decode())
        originalFileSize, compressedFileSize, compressionRatio, compressionTime = parseEncodeOutput(compressionOutput.stdout.decode())
        totalCompTime+=int(compressionTime[1])
        totalDecompTime+=int(decompressionTime[1])

        os.remove(filename + ".compressed.lzw")
        os.remove(filename+ ".compressed.lzw.decompressed.lzw")

    assert(decompressedFileSize[1] == originalFileSize[1])
    td.add("File Name", "string", filename, "The name of the file")
    td.add("Original File Size", "int", int(originalFileSize[1]), originalFileSize[0])
    td.add("Compressed Size", "int", int(compressedFileSize[1]), compressedFileSize[0])
    td.add("Compression Ratio", "flot", float(compressionRatio[1]), compressionRatio[0])
    assert(N!=0)
    td.add("Compression Time", "int", totalCompTime/N, compressionTime[0])
    td.add("Decompression Time", "int", totalDecompTime/N, decompressionTime[0])


def runCorpus(directoryName):
    # iterate over files in
    # that directory
    td = TidyData(directoryName+"Run"+sys.argv[-1], "Run of a corpus" )
    files = os.listdir(directoryName)
    for filename in files:
        td.start_record()
        file = os.path.join(directoryName, filename)
        runFile(file, td)
    td.save()
        
        
def runCorpuses():
    if not os.path.exists(LZW_ENCODE) or not os.path.exists(LZW_DECODE):
        print("Executables not available. Please run 'make all' before running this script.")
    runCorpus(CORPUS_1)
    runCorpus(CORPUS_2)



if __name__ == "__main__":
    if len(sys.argv)!=2:
        print("Please provide a number to label this run.")
        sys.exit()
    runCorpuses()