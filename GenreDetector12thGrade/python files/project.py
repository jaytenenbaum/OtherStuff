import corpusManagement
import os
from DistanceFuncs import *  # @UnusedWildImport


def main():
    cont = True
    corpusManagement.correctfiles()
    while cont == True:
        string = input("find source for string?\n(type 'n' if you don't want to search, or type the string if you do)\nNote: if you are searching for lyrics, write \"song: \" and then the lyrics\n")
        if (string != "n"):
            find_source_for_string(string, 5)
        else:
            cont = False




#######################################################################################
#######################################################################################
################################# PROGRAM FUNCTIONS ###################################
#######################################################################################
#######################################################################################


def find_source_for_string(string,n=5):
    ''' find source of inputted string'''
    sourceString=string
    jaccard_distance_list=[]## the list of all the pairs of corpus name and jaccard distance from file
    
    searchType = getSearchType(sourceString)
    corpus_files = getRelevantCorpusFiles(searchType)
    
    jaccard_distance_list, best_results = analyzeSimilarities(n, jaccard_distance_list, corpus_files, sourceString) 
    
    ## check if there is no match for word in any file
    if(not isThereAMatch(jaccard_distance_list)): 
        print("No Match!")
    
    printResults(n, jaccard_distance_list, best_results)
    promptFileOpening(best_results)
    
def getSearchType(sourceString):
    searchType = ""
    if (sourceString[0:5] == "song:"):
        searchType = "song"
    return searchType    

def find_source_for_file(file_name,n):
    '''find and print the possible sources
    of the file under the path file_name, showing only the top n
    matches and analyzing them. i.e. ("C://test.txt,3)---> ***** '''
    
    find_source_for_string(open(file_name).read(),n)
    





def top_n_results(jaccard_distance_list,n):
    '''
    (jaccard_distance_list,n)-->
    [[language_1,jaccard_distance_1],[language_2,jaccard_distance_2],...,[language_n,jaccard_distance_n]]
    a list of length n, with best compression results, with each
    element of the form [spanish_21st_century,0.766]
    
    i/o example: ([[spanish,0.9],[hebrew,0.7],[english,0.55],[romanian,0.3]],   2)--> [[spanish,0.9],[hebrew,0.7]]
    NOTE: HIGHER JACCARD IS BETTER, THERFORE THE MINUS IN THE KEY, TO SORT UP TO DOWN'''

    return sorted(jaccard_distance_list, key=lambda elem: -elem[1])[0:n]


def confidence_percentage(results,n):
    ''' gets a downward sorted list
    with each element a pair of language and jeccard distance and
    returns the array with each element the percentage of confidence
    corresponding to the compression'''
    
    a=list.copy(results)[0:n]## take n first largest pairs of [path,jaccard_distance] (note that the list is downward sorted)

    ## extract the jaccard distances of the list
    for i in range(len(a)):
        a[i]=a[i][1]## now we have all a elements as numbers that actualy represent the distance
    s=sum(a)

    ## normalize the distances, to get the probability of current source text to be the real source
    for i in range(len(a)):
        a[i]=(a[i]/s)*100
    return a

def getRelevantCorpusFiles(searchType):
    ''' returning relevant corpus files for song search, wikipedia words, etc...'''
    if (searchType == "song"):
        corpus_files = corpusManagement.getSongGenresNames()
    else:
        corpus_files = corpusManagement.getCorpusFileNames() ##get all the file paths of the files which we want to analyze similarity to
    return corpus_files


def promptFileOpening(best_results):
    question = input("do you want to open best file?: y/n\n")
    if (question == "y"):
        os.system('notepad.exe ' + best_results[0][0])



def printResults(n, jaccard_distance_list, best_results):
    ''' prints the calculated results to the screen.
    The list of closest texts, the list of how accurate the program is, and the
    probability that the searched text is one of the top results'''
    printClosestTexts(best_results)
    printProgramAccuracy(n, best_results)
    printTopResultsTotalProbability(n, jaccard_distance_list)
    
def printClosestTexts(best_results):
    print( "The List of Closest Texts:\n" + "-" * 50)
    for i in range(len(best_results)):
        print( "# {}  {} with {} % similarity".format(str(i + 1), best_results[i][0].split("\\")[-1], best_results[i][1] * 100))
def printProgramAccuracy(n, best_results):
    print( "\n\nThe List of How Accurate The Program is:\n" + "-" * 50)
    confidence_list = confidence_percentage(best_results, n)
    for i in range(len(best_results)):
        print( "# {} {} percent sure".format(str(i + 1), confidence_list[i]))
def printTopResultsTotalProbability(n, jaccard_distance_list):
    print( "\n\nProbability That it's One of The Following:\n" + "-" * 50)
    prob = sum([x[1] for x in top_n_results(jaccard_distance_list, n)]) / sum([x[1] for x in top_n_results(jaccard_distance_list, len(jaccard_distance_list))])
    print( "{}".format(prob * 100))
    
    
def analyzeSimilarities(n, jaccard_distance_list, corpus_files,sourceString, compareFunction= findJaccardDistance):
    ''' get the distances list, and the top results '''
    words= sourceString.split(' ')##list of the given words
    for corpus_file in corpus_files: ## go over all files
        currentFileString = open(corpus_file).read()
        currentFileWords = currentFileString.split(' ') ## get current file text as string
        jaccard_distance_list += [[corpus_file, compareFunction(words, currentFileWords)]] ## calculate jaccard distance between the inputted file string, and current file's one
        
    
    best_results = top_n_results(jaccard_distance_list, n) ## top n results as a form of a list, with each place a 2-long list containing current file name, and the jaccard distance to input.
    return jaccard_distance_list, best_results

def isThereAMatch(jaccard_distance_list):
    ''' find if there is at least one match in files for searched string'''
    ind = False
    for pair in jaccard_distance_list:
        if (pair[1] != 0):
            ind = True
    
    return ind
   
def return_lyrics_genre(string, compareFunction= findJaccardDistance):
    ''' find source of inputted string, using whatever function we want'''
    sourceString=string
    jaccard_distance_list=[]## the list of all the pairs of corpus name and jaccard distance from file
    
    searchType = "song"
    corpus_files = getRelevantCorpusFiles(searchType)
    
    jaccard_distance_list, best_results = analyzeSimilarities(5, jaccard_distance_list, corpus_files, sourceString, compareFunction) 
    return (best_results[0][0].split("\\")[-1]).split("LYRICS")[0][:-1:]


def testLyricsWithDifferentFunctions(string):
    ''' gets input of string, and checks it's genre with different Distance functions'''
    
    print("Searching Genre for: \n"+string+"\n")
    print("findJaccardDistance returns: "+return_lyrics_genre(string,findJaccardDistance))
    print("findJaccardWithRepetitionsDistance returns: "+return_lyrics_genre(string,findJaccardWithRepetitionsDistance))
    print("findPairsDistance returns: "+return_lyrics_genre(string,findPairsDistance))
    print("findTripletsDistance returns: "+return_lyrics_genre(string,findTripletsDistance))
    print("findQuadrupletsDistance returns: "+return_lyrics_genre(string,findQuadrupletsDistance))
    print("findHuffmanDistance returns: "+return_lyrics_genre(string,findHuffmanDistance))
    print("findAverageWordLengthDistance returns: "+return_lyrics_genre(string,findAverageWordLengthDistance)+"\n\n\n\n\n")





#testLyricsWithDifferentFunctions("my love country song cowboy sing along")
main()



