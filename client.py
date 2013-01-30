'''
A simple python client that receives jpg's over a tcp socket.

Protocol:
    1. s<buf_length>
    2. <buf>
    3. e
'''

from socket import *
from PIL import ImageFile, Image

HOST = 'localhost'
PORT = 1234

def receiveImage(sock, size):
    # use this to fill in jpg streams
    parser = ImageFile.Parser()

    jpg = sock.recv(size)
    sock.recv(1) # recv the end frame message
    parser.feed(jpg)

    img = parser.close()
    img.transpose(Image.FLIP_TOP_BOTTOM).show()

server = socket(AF_INET, SOCK_STREAM)
server.connect((HOST, PORT))

# is this client connected to a vtk server?
CONNECTED = True

while CONNECTED:
    # receive the start frame + size (1 + 4 bytes)
    data = server.recv(9)
    if not data:
        print 'socket closed at remote end'
        server.close()
    elif data[0] == 's':
        print 'Receiving image of size(bytes): ', data[1:9]
        receiveImage(server, int(data[1:9]))
    else:
        print 'expected s<buf_length> but received: ', data


