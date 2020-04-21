#! /usr/bin/python
import sys, argparse, random

class shuf:
 
 def __init__(self, numOutput, repeat, data):
  outputs = {}
  
#  print(data)
  k = 0
  while len(outputs) != len(data) and k < int(numOutput):
   if repeat == False:
    acces = random.randint(0, len(data)-1)
    while (data[acces] in outputs.keys()):
     acces = random.randint(0, len(data)-1)
    outputs[data[acces]] = 0
    print(str(data[acces]).rstrip())
   else:
    acces = random.randint(0, len(data)-1)
    print(str(data[acces]).rstrip())
   k = k + 1
  
#-------------------
def main():
 parser = argparse.ArgumentParser(description="Implementation of shuf command in python3")
 parser.add_argument('-i', '--input-range', help="specify input range", action="store", dest="iR", default="") 
 parser.add_argument('-n', '--head-count', help="specify number of items to output", action="store", dest="numlines", default=sys.maxsize)
 parser.add_argument('-r', '--repeat', help="used to allow repeat of output lines", action="store_true", dest="repeat")
 

 userInput = sys.argv[1:]
 args, unkown = parser.parse_known_args()#userInput) 
 #print (unkown)
  
 try:
  numlines = int(args.numlines)
 except:
  parser.error("invalid NUMLINES: {0}". format(args.numlines))
 if numlines < 0:
  parser.error("negative count value: {0}". format(numlines))

 try:
   if args.iR !=  "":
    indexDash = args.iR.index('-')
    if(indexDash == (len(args.iR) - 1)):
     raise NameError('ok')
    firstChunk = int(args.iR[0:indexDash])
    secondChunk = int(args.iR[indexDash+1:])
 
    if(secondChunk < firstChunk):
     raise NameError("")
    inputData = list(range(firstChunk, secondChunk+1))
    #print(inputData)
 except: 
  parser.error("invalid INPUT RANGE: {0}". format(args.iR))
# print(len(unkown)) 
 if args.iR != "" and len(unkown) != 0:
  parser.error("cannot have -i flag with a file or standard input")
 elif args.iR != "" and len(unkown) == 0:
  
  inputData = inputData
 else:
  if len(unkown) == 1 and unkown[0] == '-':
   inputData = sys.stdin.readlines()
  elif len(unkown) == 1  and unkown[0] != '-':
   try:
    curFile = open(unkown[0])
    inputData = curFile.readlines()
    curFile.close()
   except:
    parser.error("invalid file name")
  elif len(unkown) == 0:
   inputData = sys.stdin.readlines()
  else:
   parser.error("invalid input format")
 
 
 shuffler = shuf(args.numlines, args.repeat, inputData)
 

main()

