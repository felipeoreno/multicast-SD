#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "main.hpp"
#include "diagnose.hpp"

int main(int argc, char* argv[]) {
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
    std::array<char, 1024> buf;
    while(true) {
        struct Message msg(0, 0);
        buf.fill(0);
        diagnose(read(serverSocket, buf.data(), buf.size()), "Reading datagram message");
        
    }

    /*for(int i = 0; i < std::stoi(*argv); i++) { // re

    }

    // listen() switches the socket into passive mode and sets the backlog — how many completed
    // connections the kernel will queue before your code catches up by calling accept()
    listen(serverSocket, 5);
    // accept() blocks until a client connects, then returns a new file descriptor for that one connection.
    // The original socket stays open and listening, ready for the next accept() call
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    Message buffer = Message(0, 0);
    recv(clientSocket, (void *) &buffer, sizeof(buffer), 0);
    //send(serverSocket)*/

    close(serverSocket);
    return 0;
}
