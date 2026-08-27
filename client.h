#ifndef CLIENT_H
#define CLIENT_H

#include <bits/stdc++.h>

struct Client {
    int procId;
    int clock;
    std::queue<int> clocks; 

    Client(int id, int c) {
        procId = id;
        clock = c;
    }
};

struct Message {
    int procId;
    int clock;
    std::string msg;

    Message(int id, int c) {
        procId = id;
        clock = c; 
    }
};

#endif
