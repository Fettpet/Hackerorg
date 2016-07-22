import urllib 
import urllib2
import CoilBoard
import string

user = "Fettpet"
password = "hahn181991"
values = {'name' : user,
          'password' : password}

#Open URL
data = urllib.urlencode(values)
req = urllib2.Request("http://www.hacker.org/coil/index.php", data)
response = urllib2.urlopen(req)
html = response.read()


html = html[string.find(html, "FlashVars=" ) + 11: ]
boardString = html[: string.find(html, "\"") ]
board = CoilBoard.CoilBoard(boardString)
board.findDeadEnd()
board.findPassage()
board.eraseUseless(board.passageList)
board.printBoard()
board.printRegions()
print("\n\n")
board.printPassage()
board.writeToFile("/home/fettpet/coil.txt")