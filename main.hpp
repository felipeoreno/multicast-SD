#ifndef MAIN_HPP
#define MAIN_HPP

#include <bits/stdc++.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "diagnose.hpp"
#include <array>
#include<iostream>

using namespace std;

struct Message {
    int procId;
    int clock;
    std::string msg;


    Message(){
        procId = 0;
        clock = 0;
        msg = "";
    }

    Message(int id, int c, string m) {
        procId = id;
        clock = c;
        msg = m;
    }


    string message_to_string(){
        return to_string(procId) + to_string(clcok) + msg;
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

struct Client {
    int procId;
    int clock;
    std::queue<int> clocks; 

    Client(int id, int c) {
        procId = id;
        clock = c;
    }

    void send_message(string text){
        Message m = (procId, clock, text);

        int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
        diagnose(sock.isGood(), "Opening datagram socket for send");

        sockaddr_in groupSock = {};   // init to all zeroes
        groupSock.sin_family = AF_INET;
        groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
        groupSock.sin_port = htons(8080);

        in_addr localIface = {};   // init to all zeroes
        localIface.s_addr = inet_addr("127.0.0.1");
        diagnose(setsockopt(sock(), IPPROTO_IP, IP_MULTICAST_IF, (char*)&localIface,
                            sizeof(localIface)) >= 0, "Setting local interface");


        diagnose(sendto(sock(), m.message_to_string().c_str(), databuf.length(), 0,
                        (sockaddr*)&groupSock, sizeof(groupSock)) >= 0,
                "Sending datagram message");
    }

    Message delivery_message(){
        // returns a file descriptor for an IPv4 UDP socket, or a negative value on failure
        int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
        diagnose(serverSocket >= 0, "Opening datagram socket for receive");

        {
            // enable SO_REUSEADDR to allow multiple instances of this application to
            //    receive copies of the multicast datagrams.
            int reuse = 1;
            diagnose(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse,
                sizeof(reuse)) >= 0, "Setting SO_REUSEADDR");
        }

        // Bind to the proper port number with the IP address specified as INADDR_ANY
        sockaddr_in serverAddress = {};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(8080);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        // bind() attaches the socket to a local address and port. INADDR_ANY binds
        // every network interface on the machine, which is what a server usually wants
        diagnose(!bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)),
            "Binding datagram socket");

        ip_mreq group = {};    // initialize to all zeroes
        group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
        group.imr_interface.s_addr = inet_addr("127.0.0.1");
        diagnose(setsockopt(serverSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group,
            sizeof(group)), "Adding multicast group");

        // Read from the socket
        std::array<char, 1024> arr;
        arr.fill(0);
        diagnose(read(sock2(), arr.data(), arr.size()) >= 0,
                "Reading datagram message");
        std::cout << "Message from multicast sender: " << arr.data()
                    << std::endl;

        // Retorna a mensagem que foi enviada
        Mensagem m;
        m.from_datagram(arr);

        return m;
    }
};



#endif
