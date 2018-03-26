#######################################################################################
#######################################################################################
################################# DISTANCE FUNCTIONS ##################################
#######################################################################################
#######################################################################################
from Huffman_Base import compression_ratio
from collections import Counter

def findJaccardDistance(wordList1,wordList2):
    '''
    ([word_1,word_2,...,word_k],[word'_1,word'_2,...,word'_m])--->
    float Takes two lists of words, to find the jaccard distance
    between them, which is defined by the size of the intersection
    divided by the size of the union. i.e.
    (["hi","hello","one"],["hi","help","two"])--> 1/5=0.2'''
    
    ### jaccard distance between two lists of words, is the size of intersection divided by size of union###
    return len(set(wordList1) & set(wordList2))/len(set(wordList1) | set(wordList2))

def findJaccardWithRepetitionsDistance(wordList1,wordList2):
    ''' return jaccard distance taking into account the word repetitions, instead of using a set, which
    doesn't care about repetitions '''
    return len(list((Counter(wordList1) & Counter(wordList2)).elements()))/len(wordList1)

def findPairsDistance(wordList1, wordList2):
    ''' take the common subset of consecutive pair letters, divided by the union of them '''
    wordList1 = "".join(wordList1)
    wordList2 = "".join(wordList2)
    wordList1 = [(wordList1[i] + wordList1[i + 1]) for i in range(len(wordList1) - 1)]
    wordList2 = [(wordList2[i] + wordList2[i + 1]) for i in range(len(wordList2) - 1)]
    return len(set(wordList1) & set(wordList2))/len(set(wordList1) | set(wordList2))

def findTripletsDistance(wordList1, wordList2):
    ''' take the common subset of consecutive Triplet letters, divided by the union of them '''
    wordList1 = "".join(wordList1)
    wordList2 = "".join(wordList2)
    wordList1 = [(wordList1[i] + wordList1[i + 1]) for i in range(len(wordList1) - 1)]
    wordList2 = [(wordList2[i] + wordList2[i + 1]) for i in range(len(wordList2) - 1)]
    return len(set(wordList1) & set(wordList2))/len(set(wordList1) | set(wordList2))

def findQuadrupletsDistance(wordList1, wordList2):
    ''' take the common subset of consecutive Quadruple letters, divided by the union of them '''
    wordList1 = "".join(wordList1)
    wordList2 = "".join(wordList2)
    wordList1 = [(wordList1[i] + wordList1[i + 1]) for i in range(len(wordList1) - 1)]
    wordList2 = [(wordList2[i] + wordList2[i + 1]) for i in range(len(wordList2) - 1)]
    return len(set(wordList1) & set(wordList2))/len(set(wordList1) | set(wordList2))


def findHuffmanDistance(wordList1, wordList2):  
    ''' compare similarity using huffman compression ''' 
    sourceString = "".join(wordList1)
    currentFileString = "".join(wordList2)
    return compression_ratio(currentFileString, sourceString)

def findAverageWordLengthDistance(wordList1, wordList2):
    ''' compare similarity using average word length '''
    avg1=len("".join(wordList1))/len(wordList1)
    avg2=len("".join(wordList2))/len(wordList2)
    return 1-(avg1-avg2)/avg2

