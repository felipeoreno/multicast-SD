#ifndef MAIN_HPP
#define MAIN_HPP

#include <bits/stdc++.h>
#include <string>

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

    std::array<char, 1024> to_datagram() {
        std::array<char, 1024> dtg;
        char *ptr = (char *) dtg.data();
        memcpy((void *) ptr, (void *) &procId, sizeof(procId));
        ptr += sizeof(procId);
        memcpy((void *) ptr, (void *) &clock, sizeof(clock));
        ptr += sizeof(clock);
        memcpy((void *) ptr, (void *) msg.c_str(), msg.size()+1);

        return dtg;
    }

    void from_datagram(std::array<char, 1024> dtg) {
        char *ptr = (char *) dtg.data();
        memcpy((void *) &procId, (void *) ptr, sizeof(procId));
        ptr += sizeof(procId);
        memcpy((void *) &clock, (void *) ptr, sizeof(clock));
        ptr += sizeof(clock);
        //strcpy(msg.c_str(), (void *) ptr);
        msg = msg.assign(ptr);
    }
};

#endif
