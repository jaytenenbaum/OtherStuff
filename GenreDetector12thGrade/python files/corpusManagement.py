import glob
import codecs
###########################################################################
#################   ALL CORPUSES HAVE TO BE UNDER C:\corpuses #############
###########################################################################

def getCorpusFileNames():
    """ returns all the paths of the files in courpuses folder"""
    return glob.glob("C:\\corpuses/*.txt")

def getSongGenresNames():
    """ returns all the paths of the song genres in courpuses folder"""
    return glob.glob("C:\\corpuses/*LYRICS.txt")

def printBadCorpusesNames():
    """prints all the names of not readable files in corpuses folder"""
    x=glob.glob("C:\\corpuses/*.txt")
    for file in x:
        try:
            open(file).read()
        except:
            print(file)

def correctfiles():
    x=glob.glob("C:\\corpuses/*.txt")
    for file in x:
        try:
            y=open(file).read()
        except:
            x=codecs.open(file,encoding='utf-8').read()
            x="".join([ch for ch in x if ord(ch)<128])
            y=open(file,'w')
            y.truncate()
            y.write(x)
            y.close()
            try:
                open(file).read
            except:
                print("didn't work")
            print("fixed: "+file.split("//")[-1])
