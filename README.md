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