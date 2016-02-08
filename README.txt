README

Mark Rushmere
CS 372
Program 1

INCLUDED:
chatserver.cpp
chatclient.c
makefile

INSTRUCTIONS:

1. Run the makefile using the command "make".
2. Run the chat server using the command "chatserver [port number]".
3. Run the chat client using the command "chatclient [hostname] [port number]".
4. Enter a handle in chatclient.
5. Send message from chatclinet or "\quit" to terminate the connection.
6. Send a message from chatserver or "\quit" to terminate connection.
7. Repeat from 5 if connection is open.
8. Repeat from 3 if connection is closed or send SIGINT to terminate chatserver. 


SOURCES:
http://www.linuxhowtos.org/C_C++/socket.htm
http://beej.us/guide/bgnet/