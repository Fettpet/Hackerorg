import string

class CoilBoard(object):
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#				Constructor
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  def __init__(self, boardString = None):
    if(boardString == None):
      self.sizeX = 0
      self.sizeY = 0
    else:
      buffer = string.split(boardString, "=")
      
      self.deadEndList = None
      self.passageList = None
      self.sizeX = int(string.split(buffer[1], "&")[0])
      self.sizeY = int(string.split(buffer[2], "&")[0])
      grid = buffer[3]
      print(grid)
      self.grid = [[0 for x in range(self.sizeX)] for x in range(self.sizeY)] 
      for x in range(self.sizeX):
	for y in range(self.sizeY):
	  if grid[ x  + y * self.sizeX] == ".":
	    self.grid[y][x] = 0
	  else:
	    self.grid[y][x] = 1
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#		write all knowledge to file
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
  def writeToFile(self, filename):
    with open(filename, "w") as f:
      f.write(str(self.sizeX) + "\n")
      f.write(str(self.sizeY) + "\n")
      for y in range(self.sizeY):	
	for x in range(self.sizeX):
	  if( (x, y) in self.deadEndList):
	    f.write(str(3)),
	  elif( (x, y) in self.passageList):
	    f.write(str(2)),
	  elif( self.isBlocked(x,y)):
	    f.write(str(1)),
	  else:
	    f.write(str(0)),
	f.write("\n")
      #print the regions
      f.write(str(len(self.regions))+ "\n")
      for i in range(len(self.regions)):
	f.write(str(self.countRegionPassages[i]) + "\n")
	for pos in self.regions[i]:
	  if(pos in self.passageList):
	    f.write(str(pos[0]) + " " + str(pos[1]) + " ")
	f.write("\n" +str(len(self.regions[i])) + "\n")
	for pos in self.regions[i]:
	  f.write(str(pos[0]) + " " + str(pos[1]) + " ")
	f.write("\n")
	

   
#;;;;;;;;;;;;;;;;;;;;;;;;;;;;;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#		find pattern
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

# read all pattern from a pattern file
  def readPatternFile(self, file):
    currentPattern = []
    pattern = None
    with open(file, "r") as f:
      for buffer in f:
	buffer = [int(i) for i in buffer[0:len(buffer)-1]]
	if(len(buffer) == 0):
	  if(pattern == None):
	     pattern = [currentPattern]
	  else:
	    pattern += [currentPattern]
	  currentPattern =[]
	else:
	  currentPattern += [buffer]
    pattern += [currentPattern]
    return pattern
  
# find all dead ends
  def findDeadEnd(self):
    pattern = self.readPatternFile("Pattern/DeadEnd")
    self.deadEndList = self.findPattern(pattern)
    
# find all passages
  def findPassage(self):
    pattern = self.readPatternFile("Pattern/Passage")
    self.passageList = self.findPattern(pattern)
    self.eraseUseless(self.passageList)
    self.regions = self.splitTheRegions(self.passageList)

    toErase = set()
    for p in self.passageList:
      count = 0
      for r in self.regions:
	if( p in r):
	  count += 1	
      if(count == 1):
	toErase.add(p)
    for p in toErase:
      self.passageList.remove(p)
    # Count the number of passages
    self.countRegionPassages = list()
    for reg in self.regions:
      count =0
      for p in reg:
	if(p in self.passageList):
	  count += 1
      self.countRegionPassages.append(count)
    print("COunt")
    print(self.countRegionPassages)
    
#find pattern
  def findPattern(self, patternlist):
    patternPos = set()
    for patter in patternlist:
      sizeX = len(patter)
      if(sizeX == 0):
	continue
      sizeY = len(patter[0])
      
      for i in range(sizeX):
	for j in range(sizeY):
	  if(patter[i][j] == 3):
	    startX=-i
	    startY=-j
	    endX = sizeX-i
	    endY = sizeY-j
	  
      for x in range(self.sizeX):
	for y in range(self.sizeY):
	  if(self.isBlocked(x,y)):
	    continue
	  for i in range(startX, endX):
	    for j in range(startY, endY):
	      if(patter[i-startX][j-startY] == 0 and self.isBlocked(x+i, y+j)):
		break
	      if(patter[i-startX][j-startY] == 3 and self.isBlocked(x+i, y+j)):
		break
	      if(patter[i-startX][j-startY] == 2 and not self.isBlocked(x+i, y+j)):
		break
	    else:
	      continue
	    break
	  else:
	    patternPos.add((x, y))
    return patternPos
  
  def eraseUseless(self, pointSet):
    toErase = set()
    for p in pointSet:
      if((p[0] + 1, p[1]) in pointSet or self.isBlocked(p[0]+1, p[1])):
	if((p[0] - 1, p[1]) in pointSet or self.isBlocked(p[0]-1, p[1])):
	  if((p[0], p[1]+1) in pointSet or self.isBlocked(p[0], p[1]+1)):
	    if((p[0], p[1]-1) in pointSet or self.isBlocked(p[0], p[1]-1)):
	      toErase.add(p)
    for p in toErase:
      pointSet.remove(p)
    toErase.clear()
    #Erase all left 
    for p in pointSet:
      if((p[0] + 1, p[1]) in pointSet):
	toErase.add(p)
    for p in toErase:
      pointSet.remove(p)
    #erase right
    toErase.clear()
    for p in pointSet:
      if((p[0] - 1, p[1]) in pointSet):
	toErase.add(p)
    for p in toErase:
      pointSet.remove(p)
    
    #erase up
    toErase.clear()
    for p in pointSet:
      if((p[0], p[1]+1) in pointSet):
	toErase.add(p)
    for p in toErase:
      pointSet.remove(p)
    #erase up
    toErase.clear()
    for p in pointSet:
      if((p[0], p[1]-1) in pointSet):
	toErase.add(p)
    for p in toErase:
      pointSet.remove(p)
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#		split the regions
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  def splitTheRegions(self, pointSet):
    self.passageList = self.passageList.difference(self.deadEndList)
        
    finishedSet = set()
    regions = list()
    for x in range(self.sizeX):
      for y in range(self.sizeY):
	if( (x, y) in pointSet):
	  continue
	if(self.isBlocked(x, y)):
	  continue
	if( (x, y) in finishedSet):
	  continue
	regions.append(self.floodFill(pointSet, (x, y), finishedSet))

    
    return regions
    
  def floodFill(self, pointSet, start, finishedSet):
    region = set()
    region.add(start)
    toDoList = set()
    toDoList.add(start)
    while(len(toDoList) != 0):
      cur = next(iter(toDoList))
      toDoList.remove(cur)
      finishedSet.add(cur)
      x, y = cur
      if( (x+1, y) not in finishedSet and not self.isBlocked(x+1, y)):
	region.add( (x+1, y))
	if((x+1, y) not in pointSet):
	  toDoList.add((x+1, y))
      if( (x-1, y) not in finishedSet and not self.isBlocked(x-1, y)):
	region.add( (x-1, y))
	if((x-1, y) not in pointSet):
	  toDoList.add((x-1, y))
      if( (x, y+1) not in finishedSet and not self.isBlocked(x, y+1)):
	region.add( (x, y+1))
	if((x, y+1) not in pointSet):
	  toDoList.add((x, y+1))
      if( (x, y-1) not in finishedSet and not self.isBlocked(x, y-1)):
	region.add( (x, y-1))
	if((x, y-1) not in pointSet):
	  toDoList.add((x, y-1))
    return region
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#				output
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  def printPassage(self):
    for y in range(self.sizeY):
      for x in range(self.sizeX):
	if( (x, y) in self.passageList):
	  print(1),
	else:
	  print(0),
      print("")
      
      
  def printDeadEnd(self):
    for y in range(self.sizeY):
      for x in range(self.sizeX):
	if( (x, y) in self.deadEndList):
	  print(1),
	else:
	  print(0),
      print("")
  
  def printBoard(self):
    for y in range(self.sizeY):
       for x in range(self.sizeX):
	if( (x, y) in self.deadEndList):
	  print(3),
	elif( (x, y) in self.passageList):
	  print(2),
	elif( self.isBlocked(x,y)):
	  print(1),
	else:
	  print(0),
       print("")
       
  def printRegions(self):
    print(self.regions)
    for y in range(self.sizeY):
      for x in range(self.sizeX):
	if( (x, y) in self.passageList):
	  print("X"),
	elif( self.isBlocked(x,y)):
	  print("X"),
	else:
	  for reg in self.regions:
	    if( (x, y) in reg):
	      print(self.regions.index(reg)),
	      break
      print("")
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#			Help function
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  def isBlocked(self, x, y):
    if(x < 0): 
      return True
    elif(y < 0): 
      return True
    elif(x >= self.sizeX):
      return True
    elif(y >= self.sizeY):
      return True
    else:
      if(self.grid[y][x] == 1):
	return True
      else:
	return False