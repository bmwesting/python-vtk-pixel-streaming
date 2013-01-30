from socket import *
from PIL import ImageFile

HOST = 'localhost'
PORT = 1234

server = socket(AF_INET, SOCK_STREAM)
server.connect((HOST, PORT))

parser = ImageFile.Parser()

while 1:
    data = server.recv(65536)
    print 'received data'
    if not data:
        break
    parser.feed(data)

img = parser.close()
img.show()
