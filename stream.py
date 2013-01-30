from socket import *
from Visualizer import Visualizer

def socketHandler(client, addr, vis):
    pass

if __name__ == '__main__':
    HOST = 'localhost'
    PORT = 1234
    BUFSIZE = 1024
    serversock = socket(AF_INET, SOCK_STREAM)
    serversock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    serversock.bind((HOST, PORT))
    serversock.listen(1)

    print 'Waiting for client'
    clientsock, addr = serversock.accept()
    print 'Client connected from:', addr

    vis = Visualizer(clientsock)
    vis.start()
    
    # does it ever get here
    print 'Closing socket connection.'
    serversock.shutdown(1)
    serversock.close()
