import os
import subprocess
import sys
from tidydata import TidyData
import statistics

LZW_ENCODE = './lzwencode'
LZW_DECODE = './lzwdecode'
FOUR_TO_kNE_ENCODE = './four_to_one_encode'
FOUR_TO_ONE_DECODE = './four_to_one_decode'
THREE_STREAM_ENCODE = './three_stream_lzwencode'
THREE_STREAM_DECODE = './three_stream_lzwdecode'
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

    totalCompTime =[] 
    totalDecompTime = [] 
    for i in range(N):
        if(sys.argv[-1] == "-l"): 
            compressionOutput = subprocess.run([LZW_ENCODE, filename], capture_output=True)
            decompressionOutput = subprocess.run([LZW_DECODE, filename + ".compressed.lzw"], capture_output=True)
            os.remove(filename + ".compressed.lzw")
            os.remove(filename+ ".compressed.lzw.decompressed.lzw")
        elif(sys.argv[-1] == "-f"): 
            compressionOutput = subprocess.run([FOUR_TO_ONE_ENCODE, filename], capture_output=True)
            decompressionOutput = subprocess.run([FOUR_TO_ONE_DECODE, filename + ".compressed.4t1"], capture_output=True)
            os.remove(filename + ".compressed.4t1")
            os.remove(filename+ ".compressed.4t1.decompressed.4t1")
        elif(sys.argv[-1] == "-t"): 
            compressionOutput = subprocess.run([THREE_STREAM_ENCODE, filename], capture_output=True)
            decompressionOutput = subprocess.run([THREE_STREAM_DECODE, filename + ".compressed.lzw"], capture_output=True)
            os.remove(filename + ".compressed.lzw.chars")
            os.remove(filename + ".compressed.lzw.codeword")
            os.remove(filename + ".compressed.lzw.codeword.rc")
            os.remove(filename + ".tmp.compressed.lzw.codeword")
            os.remove(filename + ".compressed.lzw.indicator")
            os.remove(filename + ".compressed.lzw.indicator.rc")
            os.remove(filename + ".tmp.compressed.lzw.indicator")
            os.remove(filename + ".compressed.lzw.decompressed.lzw")
        else:
            print('Invalid option. See usage')
            sys.exit()
        decompressedFileSize, decompressionTime = parseDecodeOutput(decompressionOutput.stdout.decode())
        originalFileSize, compressedFileSize, compressionRatio, compressionTime = parseEncodeOutput(compressionOutput.stdout.decode())
        totalCompTime.append(int(compressionTime[1]))
        totalDecompTime.append(int(decompressionTime[1]))


    assert(decompressedFileSize[1] == originalFileSize[1])
    td.add("File Name", "string", filename, "The name of the file")
    td.add("Original File Size", "int", int(originalFileSize[1]), originalFileSize[0])
    td.add("Compressed Size", "int", int(compressedFileSize[1]), compressedFileSize[0])
    td.add("Compression Ratio", "flot", float(compressionRatio[1]), compressionRatio[0])
    assert(N!=0)
    td.add("Compression Time", "int", statistics.median(totalCompTime), compressionTime[0])
    td.add("Decompression Time", "int", statistics.median(totalDecompTime), decompressionTime[0])


def runCorpus(directoryName,corpusNum):
    # iterate over files in
    # that directory
    td = TidyData("corpus_"+ str(corpusNum)+ "_"+sys.argv[1], "Run of a corpus" )
    files = os.listdir(directoryName)
    for filename in files:
        td.start_record()
        file = os.path.join(directoryName, filename)
        runFile(file, td)
    td.save()
        
        
def runCorpuses():
    if not os.path.exists(LZW_ENCODE) or not os.path.exists(LZW_DECODE):
        print("Executables not available. Please run 'make all' before running this script.")
    runCorpus(CORPUS_1, 1)
    runCorpus(CORPUS_2, 2)



if __name__ == "__main__":
    if len(sys.argv)!=3:
        print("Usage\n corpus.py <run_name> <option>\n -l = lzw\n -f = four to one encoding\n -t = three stream lzw")
        sys.exit()
    runCorpuses()
