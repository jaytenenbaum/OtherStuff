###############################################################################################################################################
####################    IDEA: FOR SHIFTING PROBLEMS WHICH IT SHOWS BAD,                                                     ###################
####################    WE CAN RANDOMLY CHOSE PIXEL FROM LEFT PHOTO, AND COMPARE WITH RIGHT PHOTO, AND MOVE TO THE SIDE     ###################
####################    UNTIL THEY ARE DIFFERENT, AND FIND THE SHIFT IN X AXIS AND FIX IT.                                  ###################
###############################################################################################################################################
import Image

def start():
    src=raw_input("Specify full name (with .bla) of the file (recommended to finish with png)\n")
    name = src.split(".")[0]
    try:
        # Load an image from the hard drive
        original = Image.open(src)
        print 'opened'
        firstPhoto= getLeftHalf(original)
        firstPhoto.save(name+"1_leftHalf.png")
        print 'got left half'
        secondPhoto= getRightHalf(original)
        secondPhoto.save(name+"1_rightHalf.png")
        print 'got right half'
        minusPhoto = getDiff(firstPhoto, secondPhoto)
        minusPhoto.save(name+"2_minusPhoto.png")
        print 'got minus'
        bwPhoto = blackWhite(minusPhoto)
        bwPhoto.save(name+"3_bwPhoto.png")
        print 'got b&w'
        cleaned = clean(bwPhoto)
        cleaned.save(name+"4_cleanedPhoto.png")
        borderPixels = getBorders(cleaned)
        borderedBW= addBwBorders(cleaned,borderPixels)
        print 'got b&w bordered'
        borderedBW.save(name+"5_borderedBW.png")
        output = markBorder(original, borderPixels)
        output.save(name+"6_solution.png")
        
    except:
        print "Unable to load image"

def getLeftHalf(photo):
    (w,h) = photo.size
    assert(w%2==0)
    
    res = Image.new("RGB", (w/2, h), "white")
    for x in range(w/2):
        for y in range(h):
            res.putpixel((x,y),photo.getpixel((x,y)))
    return res

def getRightHalf(photo):
    (w,h) = photo.size
    assert(w%2==0)
    res = Image.new("RGB", (w/2, h), "white")
    for x in range(w/2):
        for y in range(h):
            res.putpixel((x,y),photo.getpixel((x+w/2,y)))
    return res

def getDiff(firstPhoto, secondPhoto):
    (w,h) = firstPhoto.size
    res = Image.new("RGB",(w,h), "white")
    for x in range(w):
        for y in range(h):
            res.putpixel(  (x,y)    ,  subtr( firstPhoto.getpixel((x,y)) , secondPhoto.getpixel((x,y)) )        )
    return res

def blackWhite(minusPhoto):
    (w,h) = minusPhoto.size
    res = Image.new("RGB",(w,h), "white")
    for x in range(w):
        for y in range(h):
            if (minusPhoto.getpixel((x,y))==(0,0,0)):
                res.putpixel((x,y),(0,0,0))
            else:
                res.putpixel((x,y), (255,255,255))
    return res


def isBlack(photo,x,y):
    return photo.getpixel((x,y))==(0,0,0)



def subtr(tp1,tp2):
    (a1,b1,c1)=tp1
    (a2,b2,c2)=tp2
    ret= ((a1-a2)%255,(b1-b2)%255,(c1-c2)%255)
    return ret


def clean(bwPhoto):
    (w,h) = bwPhoto.size
    res = Image.new("RGB",(w,h), "white")
    for x in range(3,w-3):
        for y in range(3,h-3):
            median =getMedianVal(bwPhoto,x,y)
            if (median==0):
                res.putpixel((x,y),(0,0,0))
            else:
                res.putpixel((x,y), (255,255,255))
    return res

def getMedianVal(photo,x,y):
    vals=[]
    for i in range(x-3,x+4):
        for j in range(y-3,y+4):
            vals+=[photo.getpixel((i,j))[0]]
    return median(vals)

            
def median(mylist):
    sorts = sorted(mylist)
    length = len(sorts)
    if not length % 2:
        return (sorts[length / 2] + sorts[length / 2 - 1]) / 2.0
    return sorts[length / 2]

def getBorders(photo):
    (w,h) = photo.size
    borders=[]
    for x in range(2,w-2):
        for y in range(2,h-2):
            if (bordersWhite(photo,x,y)):
                borders+=[(x,y)]
    return borders

    
def bordersWhite(photo,x,y):
    if not photo.getpixel((x,y))[0]==0:
        return False
    for i in range(x-2,x+3):
        for j in range(y-2,y+3):
            if not photo.getpixel((i,j))[0]==0:
                return True
    return False


def markBorder(original, borderPixels):
    (w,h) = original.size
    res = original.copy()
    for (a,b) in borderPixels:
        res.putpixel((a,b), (255,0,0))
        res.putpixel((a+w/2,b), (255,0,0))
    return res

def addBwBorders(original,borderPixels):
    (w,h) = original.size
    res = original.copy()
    for (a,b) in borderPixels:
        res.putpixel((a,b), (255,0,0))
    return res

start()
