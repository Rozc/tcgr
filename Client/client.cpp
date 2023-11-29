//
// Created by UserName on 2023/11/28.
//

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <vector>


int task(int id) {
    int clientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientFd == -1) {
        std::cout << "Create Socket Failed" << std::endl;
        exit(-1);
    }

    std::string host = "host.docker.internal";
    hostent* ht = gethostbyname(host.c_str());
    if (ht == nullptr) {
        std::cout << "Get Host By Name Failed" << std::endl;
        exit(-1);
    }

    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(32805);
    serverAddr.sin_addr.s_addr = *(in_addr_t*)ht->h_addr_list[0];
    if (connect(clientFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cout << "Connect Failed" << std::endl;
        exit(-1);
    }
    std::cout << id << " connected." << std::endl;
    std::string msg = "Hello, World! " + std::to_string(id) + "th thread.";
    std::cout << msg << std::endl;
    sleep(2);
    write(clientFd, msg.c_str(), msg.size());
    sleep(2);
    write(clientFd, msg.c_str(), msg.size());
    sleep(2);
    close(clientFd);
    return 0;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::vector<std::thread> threads;
    int n = 1;
    for(int i=0; i<n; i++) {
        threads.emplace_back(task, i);
    }

    for(int i=0; i<n; i++) {
        threads[i].join();
    }

    char buffer[1024];

//    std::string input;
//    while (true) {
//        std::cin >> input;
//        send(clientFd, input.c_str(), input.size()+1, 0);
//    }
}
