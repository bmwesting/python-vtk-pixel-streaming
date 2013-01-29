from socket import *

HOST = 'localhost'
PORT = 1234

server = socket(AF_INET, SOCK_STREAM)
server.connect(HOST, PORT)
