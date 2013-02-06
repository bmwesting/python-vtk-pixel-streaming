#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

#include "socket.h"

// Creates a TCP socket and return the socket descriptor
int tcp_connect(char* hostname, int port)
{
    int sd; // file descriptor index
    struct sockaddr_in s_address; // server
    struct sockaddr_in c_address; // client
    struct hostent *hp;
    int rv;
    
    // --- Create a socket connection
    
    // IP lookup
    hp = gethostbyname(hostname);
    if (hp)
    {
        printf("host found: %p\n", hp);
        printf("host found: %s\n", hp->h_name );
    }
    else
    {
        printf("host not found\n");
        return -1;
    }

    // fill socket structure
    memset(&s_address, 0, sizeof(s_address));
    s_address.sin_family = AF_INET;
    memcpy(&s_address.sin_addr, hp->h_addr, hp->h_length);
    s_address.sin_port = htons(port);

    memset(&c_address, 0, sizeof(c_address));
    c_address.sin_family = AF_INET;
    c_address.sin_addr.s_addr = htonl(INADDR_ANY);
    c_address.sin_port = htons(port);

    // set timeout on socket
    // TODO: Parameterize
    struct timeval timeout;
    timeout.tv_sec = 0;     // seconds
    timeout.tv_usec = 500000; // micro seconds ( 0.5 seconds)
    setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval)); 

    // create the socket
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        printf("Unable to create socket.\n");
        return -1;
    }
    else    
    {
        //printf("socket is opened: %i \n", sd);
        /*
        rv = fcntl(sd, F_SETFL, O_NONBLOCK); // socket set to NONBLOCK
        if(rv < 0)
            printf("nonblock failed: %i %s\n", errno, strerror(errno));
        else
            printf("socket is set nonblock\n");
        */
    } 

    // bind socket
    rv = bind(sd, (struct sockaddr *) &c_address, sizeof(c_address));
    if (rv < 0)     {
        printf("ERROR bind() %i: %s\n", errno, strerror(errno));
        return -1;
    }

    // connect to server
    rv = connect(sd, (struct sockaddr *) &s_address, sizeof(s_address));
    printf("rv = %i\n", rv);
    if (rv < 0 && errno != EINPROGRESS)     {
        printf("ERROR connect() %i:  %s\n", errno, strerror(errno));
        return -1;
    }
    else
        printf("Connected to %s.\n", hostname);

    return sd;
}

// Receives <length> bytes from socket, blocking until finished
int tcp_recv(int sd, unsigned char* buf, int length)
{

    int bytes_remaining = length;
    int received = 0;

    while(bytes_remaining != 0)
    {
        received = recv(sd, &buf[length - bytes_remaining], 
                                                bytes_remaining, 0);
        if (received == -1)
        {
            perror("Receive error on Message.");
            return -1;
        }
        else if (received == 0)
        {
            printf("Server closed connection.\n");
            return -1;
        }

        bytes_remaining -= received;
        //printf("Received %d bytes. %d remaining.\n",received, bytes_remaining); 
    }

    return 0;
}
