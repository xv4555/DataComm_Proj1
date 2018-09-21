# DataComm_Proj1
Data Communications and Networks Project 1

compile:
gcc 351ChatClient.c -o 351ChatClient
gcc 351ChatServer.c -o 351ChatServer

example run:
./351ChatServer 8080 11
./351ChatClient 127.0.0.1 8080 NAME PASSWORD

Assumptions:
name and password dont have spaces
name and password are each <=500 bytes
messages are <=1000 bytes

---

Client high level approach:

1) get info from user
2) connect to server
3) send name/password
4) recieve confirmation
5) infinite loop reading in input and messages from the server

---

Server high level approach:

1) array of client sockets
2) accept client connections
3) infinite loop checking activity for more connections or message from client
4) broadcast message

---

Challenges:

-client reading input from stdin and messages from server at the same time
-data sent smaller than buffer size -> garbage data / trailing 0's
-preserving data for each client

---

How we tested our code:

-ran the client/server with wrong/bad arguments
-ran the client without the server being up
-Ctrl+c quit the client program instead of #EXIT
-ran more than the max # of clients
