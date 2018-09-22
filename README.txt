Names: David Zou
       Abraham Glasser
# DataComm_Proj1
Data Communications and Networks Project 1

compile:

gcc 351ChatClient.c -o 351ChatClient <br />
gcc 351ChatServer.c -o 351ChatServer <br />

example run:

./351ChatServer 8080 11 <br />
./351ChatClient 127.0.0.1 8080 NAME PASSWORD <br />

Assumptions:

name and password dont have spaces <br />
name and password are each <=500 bytes <br />
messages are <=1000 bytes <br />

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

-client reading input from stdin and messages from server at the same time <br />
-data sent smaller than buffer size -> garbage data / trailing 0's <br />
-preserving data for each client <br />

---

How we tested our code:

-ran the client/server with wrong/bad arguments <br />
-ran the client without the server being up <br />
-Ctrl+c quit the client program instead of #EXIT <br />
-ran more than the max # of clients <br />
