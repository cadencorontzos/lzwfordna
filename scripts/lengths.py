
import sys
import random

def dictIndex(stringLength):
    return min(stringLength-2, 16)


def findCommonSubstrings(filename):
    
    file        = open(filename, 'r')

    seen = [dict() for _ in range(17)]
    nextChar = file.read(1) 
    length = 0
    currentSubstring = ""
    while nextChar:  
        currentSubstring+=nextChar
        length+=1
        index = dictIndex(length)
        if length>=2:
            if currentSubstring in seen[index]:
                seen[index][currentSubstring]+=1
            else:
                seen[index][currentSubstring] =1 
                length = 0
                currentSubstring=""
        nextChar = file.read(1)


    reportMostSeen(seen)
    file.close()
    

def reportMostSeen(seen):
    print(seen[2])
    for i in range(2,17):
        
        index = dictIndex(i)
        print("Most seen strings of length " +str(i)  + ":")
        for j in range(3):
            maxKey = max(seen[index], key=seen[index].get)
            print(maxKey)
            seen[index][maxKey] = 0
if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit("Please provide file name")
        
    findCommonSubstrings(sys.argv[1])

