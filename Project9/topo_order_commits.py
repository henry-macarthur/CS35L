# Keep the function signature,
# but replace its body with your implementation
import os
import zlib
import sys

#sys.setrecursionlimit(10**8)

def find_all(inp, sub):
    st = 0
    while True:
        st = inp.find(sub, st)
        if st == -1: return
        yield st
        st += len(sub)

class CommitNode:
   def __init__(self, commit_hash):
      """
      :type commit_hash: str
      """
      self.commit_hash = commit_hash
      self.parents = set()
      self.children = set()
      self.visited = False
      self.deg = 0

def DFS_Iterative(startNode, node_dict, topoOrder):
    stack1 = []
    stack2 = []
    added = set()

    stack1.append(node_dict[startNode.commit_hash])

    while(len(stack1) != 0):
        curNode = stack1.pop()

        stack2.append(curNode.commit_hash)

        for child in sorted(curNode.children):
            stack1.append(node_dict[child])

    while(len(stack2) != 0):
        item = stack2.pop()
        if item not in added:
            topoOrder.append(item)
            added.add(item)

#---------------------------
def DFS_Recursive(startNode, node_dict, topoOrder): #this is not used int eh program anymore!

    if len(startNode.children) == 0 and startNode.visited == False and startNode.commit_hash not in topoOrder:
        topoOrder.append(startNode.commit_hash)
        startNode.visited == True
        return
    elif(startNode.visited == True):
        #topoOrder.append(startNode.commit_hash)
        return

    #search all of the nodes children
    startNode.children = sorted(startNode.children)
    for child in startNode.children:
   #     print(child)
        childNode = node_dict[child] #get the node from the hash table
        DFS2(childNode, node_dict, topoOrder)

    if startNode.commit_hash not in topoOrder:
        topoOrder.append(startNode.commit_hash)
    startNode.visited = True
#--------------------------


def DFS_Helper(nodes, node_dict, hash_to_branch):
    visited = set()
    topoOrder = []
    #print("hi")
    for leaf in nodes:
        DFS_Iterative(node_dict[leaf], node_dict, topoOrder)

    lastN = False

    for i in range(len(topoOrder)):

        if lastN == True:
            #print('....')
            #print(topoOrder[i])
            print("=", end ="")
            for item in node_dict[topoOrder[i]].children:
                print(item, end =" ")
            print("")

            str = " "
            if(topoOrder[i] in hash_to_branch):
                #print(topoOrder[i])
                to_print = sorted(hash_to_branch[topoOrder[i]])
                for hash in to_print:
                    str += hash
                    str += " "

            print(topoOrder[i] + str)
            str = " "
            #print(topoOrder[i])
            lastN = False
            continue
        else:
            str = " "
            if(topoOrder[i] in hash_to_branch):
                #print(topoOrder[i])
                to_print = sorted(hash_to_branch[topoOrder[i]])
                for hash in to_print:
                    str += hash
                    str += " "

            print(topoOrder[i] + str)
            str = " "
        #fix this so it is all on the same line
        if(i <= len(topoOrder)-2):
            nextC = topoOrder[i+1]
            if nextC not in node_dict[topoOrder[i]].parents:
                #print(len(node_dict[topoOrder[i]].parents) -1)
                if len(node_dict[topoOrder[i]].parents) != 0:
                    counter = 0
                    for parent in node_dict[topoOrder[i]].parents:
                        if counter != (len(node_dict[topoOrder[i]].parents) -1 ):
                            print(parent, end = " ")
                        else:
                            #print ("...", end = " ")
                            print(parent + '=')
                            print("")
                            lastN = True
                            break

                        counter = counter +1
                else:
                    print("=")
                    print("")
                    lastN = True
                    break



def DFS(startNode):
   visited = set()
   node_dict = {}
   stack = []
   rootCommit = ''

   node_dict[startNode.commit_hash] = startNode
   stack.append(startNode)

   while(len(stack) != 0):

      cur = stack.pop()

      if cur.commit_hash not in visited:
      #    #do something
         visited.add(cur.commit_hash)

      #print(cur.commit_hash)
      for parent in cur.parents:

         if parent not in visited:
            # at this point we know the parent has not been visited
            # must make a new node with this as the hash
            newNode = CommitNode(parent)
            newNode.children.add(cur.commit_hash)

            folder = parent[0:2] # first two bytes = folder
            com = parent[2:len(parent)] #commit hash

            comFile = open('./objects/' + folder + '/' + com, 'rb') # open the commit data

            commit = comFile.read() # read commit data
            data = str(zlib.decompress(commit)) # get the decoded info
            # need to find its parents :(

            if "parent" in data:
               parentsI = list(find_all(data, "parent"))

               for j in range(len(parentsI)):
                  newNode.parents.add(data[int(parentsI[j]) + 7: int(parentsI[j]) + 47])
                  #print(len(parentsI))
            else:
               #this means that this is a root_commit and we should add it to the root commits list
               rootCommit = newNode
               #print(rootCommit.commit_hash)

            stack.append(newNode)
            #for index in newNode.parents:
               #stack.append(index.commit_hash)

            node_dict[parent] = newNode #add the node to the dictionary
         else:
            node_dict[parent].children.add(cur.commit_hash)
            #print("ok")
            #NOW have a list of all the parents!
   return [rootCommit, node_dict]

def topo_order_commits():
   root_commits = set()
   dictionary = {}

   curDir = next(os.walk('.'))[1] # get all of directories in current dir

   while ".git" not in curDir:

      if os.getcwd() == "\\":
         sys.stderr.write("Not inside a Git Repository \n")
         exit(1)
      try:
          os.chdir('..')
          curDir = next(os.walk('.'))[1]
      except:
          print("Not inside a Git Repository \n", file=sys.stderr)
          exit(1)

   #should be inside the directory that contains .git, so cd into it now

   os.chdir(".git");

   branches = next(os.walk('./refs/heads'))[2]

   hash_to_branch = {}

   #now have a list of all the branches

   for item in range(len(branches)):
      f=open('./refs/heads/' + branches[item], 'r') #open each branch
      f1 = f.readlines() # get start commits
      for c in range(len(f1)):

          folder = f1[c][0:2] # first two bytes = folder
          com = f1[c][2:len(f1[c])-1] #commit hash

          startCommit = CommitNode(folder + com) #create the starting node

          if (folder + com) not in hash_to_branch:
              hash_to_branch[folder+com] = set()

          hash_to_branch[folder+com].add(branches[item])
          comFile = open('./objects/' + folder + '/' + com, 'rb') # open the commit data

          commit = comFile.read() # read commit data
          data = str(zlib.decompress(commit)) # get the decoded info
          #print(data) # can delete later
          if "parent" in data:
             parentsI = list(find_all(data, "parent"))


             for j in range(len(parentsI)):
                startCommit.parents.add(data[int(parentsI[j]) + 7: int(parentsI[j]) + 47])

        #  print("........" + branches[item])
          #print(startCommit.commit_hash)
          returnValue = DFS(startCommit)
          rootCommit = returnValue[0]
          #print("---")
          #print(rootCommit.commit_hash)
          dict = returnValue[1]
          root_commits.add(rootCommit.commit_hash)
          for key in dict:
              if key in dictionary:
                  #have to add the children and parents
                  for child in dict[key].children:
                      dictionary[key].children.add(child)
                  for parent in dict[key].parents:
                      dictionary[key].parents.add(parent)
              else:
                  dictionary[key] = dict[key]
          #this should take care of overlapping branches etc!

         
   DFS_Helper(root_commits, dictionary, hash_to_branch)
       #want to perform dfs!









if __name__ == '__main__':
    topo_order_commits()