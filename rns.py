
import sys
from tidydata import TidyData
import random

N=3

def encodeRNS(s):
    originalNum = ord(s)
     
    moduloSet = [2**N -1, 2**N, 2**N+1]

    r0 = originalNum % moduloSet[0]
    r1 = originalNum % moduloSet[1]
    r2 = originalNum % moduloSet[2] 
    return r0, r1, r2

def decodeRNS(r1, r2, r3):
    moduloSet = [2**N -1, 2**N, 2**N+1]
    return chr(crt(r1, r2, r3, moduloSet))

def crt(r1, r2, r3, moduloSet):
    bigN = moduloSet[0] * moduloSet[1] * moduloSet[2]
    
    n1 = bigN//moduloSet[0]
    n2 = bigN//moduloSet[1]
    n3 = bigN//moduloSet[2]

    y1 = mod_Inv(n1, moduloSet[0])
    y2 = mod_Inv(n2, moduloSet[1])
    y3 = mod_Inv(n3, moduloSet[2])

    return ((y1*r1*n1)+(y2*r2*n2)+(y3*n3*r3))%bigN

# I know Extended Eucl. Alg. is faster but this is just so much easier for now
def mod_Inv(x,y):
    for i in range(y):
        if (x*i)%y==1:
            return i

# def decimalToBinary(n):
#     s = bin(n).replace("0b", "0")
#     return ("0" * ((3*N - len(s)))) + s

def printReport(c):
    print(c)
    print("The original character is " + c+ ".")
    r1, r2, r3 = encodeRNS(c)
    print("In RNS it is ", r1, r2, r3)
    dec = decodeRNS(r1, r2, r3)
    print("Decoded it is " + dec + "." )
    assert(c==dec)

def toRNSStream(filename):
    td = TidyData(filename+ " rns Stream", "Contains rns data for a text file" )
    
    file        = open(filename, 'rb')
    newFile1     = open(filename + ".one.txt", 'w')
    newFile2    = open(filename + ".two.txt", 'w')
    newFile3    = open(filename + ".three.txt", 'w')

    nextChar = file.read(1)
    while nextChar:  
        td.start_record()
        r1, r2, r3 = encodeRNS(nextChar)
        # nextChar = file.read(1)
        # if not nextChar:
        #     break
        # d1, d2, d3 = encodeRNS(nextChar)
        # newFile1.write(combineStreamsToByte(r1, d1))
        # newFile2.write(combineStreamsToByte(r2, d2))
        # newFile3.write(combineStreamsToByte(r3, d3))
        newFile1.write(str(r1))
        td.add()
        newFile2.write(str(r2))
        newFile3.write(str(r3))
        nextChar = file.read(1)

    newFile1.close()
    newFile2.close()
    newFile3.close()
    file.close()
    td.save()

def combineStreamsToByte(s1, s2):
    combo = s1* (2**4) +s2
    return combo.to_bytes(1, byteorder='big')

def produceDistributionReport():
    randPrefix = random.randint(10000,20000)
    

if __name__ == "__main__":
    # for i in range(255):
    #     printReport(chr(i))
    toRNSStream('./textfiles/wap.txt')

