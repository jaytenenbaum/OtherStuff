#########################################################################################
## Things it can do: you can log on to different users, and make users of your own.
## each user has files of it's own, which you can edit with the editor. a file for username
## "u" for example will be "u- test". we can browse files for each user.
## I also added a calculator, which does calculations recursively.
#########################################################################################



## how stats work: when we first create the user, it defines userStats, and adds default line to Stats.txt. When we log in, we import the information from Stats.txt
##Then every time there is an update in one of the stats,
## we just update the list userStats, and then before exit we update the file Stats.txt



import sys
import time
import glob
import os
import msvcrt
from threading import Thread
global Spaces## long sequence of spaces
Spaces="\n"*50
currentUser=""## the current user logged on
global statOrder
statOrder={"Username": 0,"Total time logged on":1,"Amount of times logged in":2,"Amount of files":3}## used to access the different criterias of the statistics
userStats=[0,0,0,0]## the stats of the user (given default value). represents: [username, total time, login number, amount of files]
global startSessionTime## the time.time() of which we started the session


def fileBrowser():
    ''' Browse the different files on the user, and edit one of them, or return back to main menu.'''
    
    print(Spaces)
    fileNames=glob.glob('*.txt')## take only txt files
    print("File Names:\n")
    
    relevantFiles=[]## all the files for current user

    ## print current user files, and fill up "relevatFiles"
    i=1
    for fname in fileNames:
        if fname.startswith(currentUser+"- "):
            print(str(i)+". "+fname[len(currentUser)+2:-4])
            i+=1
            relevantFiles+=[fname]
    a=raw_input("\nPress 'Enter' to return...\nOr press the number of the file to edit it.\n")

    ## open the chosen files by using inputted number, or if no inputted number return to main menu
    try:
        num=int(a)
        print(Spaces+"Editing the file: "+relevantFiles[num-1][len(currentUser)+2:-4])
        editFile(relevantFiles[num-1])
        menu()
    except:
        menu()

    

    
def editFile(fileName):
    ''' Edit a file using its name. It opens the file, and lets user to input row by row the additionl part of file. Then saves it.'''
    global userStats
    
    currentFile= open(fileName,'a+')## open file for read and write
    if len(currentFile.read())>0:
        currentFile.seek(0)## start reading from beginning
        currentFileTextOnOpen="".join(currentFile.read().split("<"))## take out the "<"s in the real file
        print("The file is: \n"+currentFileTextOnOpen+"\nContinue typing the File below:")
        currentFile.seek(0,2)## go back to the end of the file for writing
        
    else:
        print("A new file has been created.\nType the File below:")
        userStats[3]+=1
    userInput=""
    
    ## while user keeps inputing lines, and doesn't want to save and return using '<'
    while not (userInput == '<' ):
        userInput=raw_input("")
        currentFile.write(userInput+"\n")
    ## save and return to main menu
    currentFile.close()
    print("Saving....")
    time.sleep(0.5)
    print("Saved!")
    time.sleep(0.3)
    
def textEditor():
    ''' Create a new file or edit and existing one of current user'''
    
    fileName= currentUser+"- "+raw_input(Spaces+"Welcome to Text Editor:\nType '<' anytime to back out.\nInput file name: \n")+".txt"
    editFile(fileName)
    menu()

def calculate(st):
    ''' Calculates the value of a string, using recursion. It splits the text by the different operations, by the right order, and then recursively calculates,
    until the input is an integer, and it just returns its integer value.'''
    
    splitted=False
    
    st= st.split('+')
    if len(st)>1:
        splitted=True
        s=0
        for elem in st:
            s+=calculate(elem)
        return s
    else:
        st=st[0]

    st= st.split('-')
    if len(st)>1:
        splitted=True
        s=0
        for elem in st:
            s-=calculate(elem)
        return s
    else:
        st=st[0]
        
    st= st.split('*')
    if len(st)>1:
        splitted=True
        s=1
        for elem in st:
            s*=calculate(elem)
        return s
    else:
        st=st[0]
    st= st.split('/')
    if len(st)>1:
        splitted=True
        s=st[0]**2
        for elem in st:
            s/=calculate(elem)
        return s
    else:
        st=st[0]

    st= st.split('^')
    if len(st)>1:
        splitted=True
        return calculate(st[0])**calculate(st[1])
    else:
        st=st[0]

    if splitted==False:
        return int(st)
    
def calculator():
    ''' The calculator application.'''
    
    i=0 ## just used to know if we need the spaces for the first calculation
    while True:
        if i==0:## only print spaces for first calculation
            print(Spaces+"Calculate:")
        else:
            print("\nCalculate:")
        userInput= raw_input("Type < to return to Main Menu\n")
        if userInput=='<':
            menu()
        for ch in userInput:## check if there aren't unknown characters
            if ch not in ['1','2','3','4','5','6','7','8','9','0','.','+','-','*','/','^','(',')']:
                print("Input error, Try again!")
        print(calculate(userInput))## calculate the string's mathematical value
        i+=1

def StopperWatch():
    t0=time.time()
    while True:
        try:
            time.sleep(0.1)
            print(Spaces+"Press ctrl+c to return to main menu:\n"+"Time is: "+"%.3f"%(time.time()-t0))
        except KeyboardInterrupt:
            print(Spaces+"Finished: "+"%.3f"%(time.time()-t0))
            time.sleep(2)
            menu()
        



    
def getStats():
    '''The application which prints the statistics of current user.'''
    
    global startSessionTime
    print(Spaces)
    for cret in statOrder:## go over all creterias for statistics
        if cret!="Total time logged on":
            try:## try to print the integer version of the statistics (instead of 2.0 or 4.0 login times)
                print(cret+ ": "+str(int(userStats[statOrder[cret]])))
            except:
                print(cret+ ": "+str(userStats[statOrder[cret]]))
        else:## used to show current login time, because we only set or get total time on startup and on exit, therefore we add to the last total the current time minus login time.
            print(cret+ ": "+str(int(userStats[statOrder[cret]]+time.time()-startSessionTime))+" Seconds")
    print("")
        
    a=raw_input("Press 'Enter' to return.")
    menu()
            
def updateStats(user):    
    ''' Before exiting, update the txt file with the new stats using the list userStats. The current time is already updated, so no reason to play with it.'''
    
    global userStats
    f = open("Stats.txt", 'r+')## open stats file
    f.truncate()
    for i in range(4):## go over stats and convert them to strings in able to store in stats file.
        userStats[i]=str(userStats[i])
    f.write(" ".join(userStats)+"\n")## write stats to file
    

def changePassword():
    newPass=raw_input("New password: \n")
    f=open("passwords.txt",'r')
    text=f.read()
    f.close()
    text=text.split("\n")[:-1]
    i=0
    for row in text:
        if row.split(",")[0]==currentUser:
            text[i]=row.split(",")[0]+","+newPass
        i+=1
    f=open("passwords.txt",'w')
    f.write("\n".join(text)+"\n")
    f.close()
    menu()
    

def menu():
    ''' The main menu of the OS. The access point for all applications.'''
    global startSessionTime
    try:
        choice=raw_input(Spaces+"You are currently at Main Menu: \n1. Switch User\n2. Calculator\n3. Text Editor\n4. File Browser\n5. Stopper Watch\n6. User Statistics\n7. Change Password\n\nPress Ctrl+c to exit.\n")
        while choice not in ["1","2","3","4","5","6","7"]:
                choice=raw_input("Try again!")
        if choice=="1":
            userStats[1]+=time.time()-startSessionTime
            updateStats(currentUser)
            login()
        if choice=="2":
            calculator()
        if choice=="3":
            textEditor()
        if choice=="4":
            fileBrowser()
        if choice=="5":
            StopperWatch()
        if choice=="6":
            getStats()
        if choice=="7":
            changePassword()
    except KeyboardInterrupt:
        print(Spaces+"Thanks for using JayOS!")
        userStats[1]+=time.time()-startSessionTime
        updateStats(currentUser)
        sys.exit()

        
def doesExist(user):
    ''' Check if a given user exists on OS'''
    
    try:
        f=open("passwords.txt",'r').read()## check for user in passwords file
        f=f.split("\n")
        for userAndPass in f[0:-1]:## iterate over f
                if userAndPass.split(",")[0]==user:
                    return True
        return False
    except:## no users yet
        return False
        
def createDefaultStats(user):
    ''' Creates default stats for just created user. it both reset the variable userStats, and adds a new row in the file "Stats.txt" '''
    global userStats
    userStats=[user,0,0,0]
    f=open("Stats.txt",'a+')
    f.write(user+" 0 0 0\n")
    f.close()

    
def createNewUser():
    ''' Prompt user to create a new user.'''
    
    print("\nCreate new user:")
    user= raw_input("Username: ")## get new user
    while doesExist(user):
        print("The user already exists! Try another one: ")
        user= raw_input("Username: ")
    password=raw_input("Password: ")## get new password

    ## add new user and password to the passwords file
    f=open("passwords.txt",'a')
    f.write(user+","+password+"\n")
    f.close()

    ## finish and prompt login
    print("Successfully created user.")
    time.sleep(2)
    createDefaultStats(user)
    login()

def setCurrentUser(user):
    ''' Set the current user for OS to be the inputted one.'''
    global currentUser
    currentUser=user

    
def loadStats(user):
    ''' Load the stats from the file "Stats.txt" to the list userStats. '''
    global userStats

    for userLine in (open("Stats.txt").read()).split("\n"):

        if userLine.split(" ")[0]==currentUser:
            userStats=userLine.split(" ")
            for i in range(1,4):
                userStats[i]=float(userStats[i])

            
            
    
def login():
    ''' The login screen for the OS.'''
    global userStats
    global startSessionTime
    
    print(Spaces+"Welcome To Jay's OS!")
    decision=raw_input("To create new User Type 'new'\nTo login Press 'Enter'...\n")
    ## if decided to login
    if decision=="":
        print(Spaces)
        user=raw_input("\nPlease fill in Username and Password:\nUsername: ")
        password=raw_input("Password: ")

        ## try to open the password file, if exists- (only exists if there is already a user), and look for user and password, which are split by a comma in a line of their own.
        try:
            f=open("passwords.txt",'r').read()
            f=f.split("\n")## set f as the list: ["user1,password1","user2,password2","user3,password3",...,""] (because the file ends with an empty line)
            for userAndPass in f[0:-1]:## iterate over f
                if userAndPass.split(",")[0]==user and userAndPass.split(",")[1]!=password:## if the user exists but the password is wrong
                    print("Wrong password, Try again!")
                    time.sleep(2)
                    login()
                if userAndPass.split(",")[0]==user and userAndPass.split(",")[1]==password:## if the user and password are correct.
                    setCurrentUser(user)
                    loadStats(currentUser)
                    time.sleep(1)
                    userStats[2]+=1
       
                    startSessionTime=time.time()
                    menu()
                    
            ## if no such user
            print("No such user!")
            ## prompt creation of new user, or retry login.
            decision= raw_input("Do you want to create new user? (y/n) ")
            if decision=="y":
                createNewUser()
            else:
                login()
        ## no 'passwords.txt' file, therefore no users in os. Prompt creation of new user.
        except IOError:
            print("No users found in OS.")
            createNewUser()
    ## if decided to create new user
    else:
       print(Spaces)
       createNewUser()


############################################################## INITIATE OS
login()
