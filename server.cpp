#include <iostream>
#include <winsock2.h> 
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")
using namespace std;
//save chat history
ofstream logFile("server_chat_history.txt", std::ios::app);
void LogMessage(const std::string& message) {
    logFile << message << std::endl;
}

void InteractWithClient(SOCKET clientSocket, vector<SOCKET>& clients) {
    //send/recieve client
    cout << "client connected" << endl;
    char buffer[4096];

    while (1) {
        int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesrecvd <= 0) {
            cout << "client disconnnect" << endl;
        }
        string message(buffer, bytesrecvd);
        cout << "message from client: " << message << endl;

        // Log the message
        LogMessage("Client: " + message);

        for (auto client : clients) {
            if (client != clientSocket) {
                send(client, message.c_str(), message.length(), 0);
            }
        }
    }
    
    auto it = find(clients.begin(), clients.end(), clientSocket);
    if (it != clients.end()) {
        clients.erase(it);
    }
    closesocket(clientSocket);
}



int main() {
    WSADATA wsaData; 
    //Socket- data is sent and received.
    
    // Step 1: Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }
    //cout << "Winsock initialized successfully!" << endl;

    // Step 2: Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket == INVALID_SOCKET) {
        cout << "Socket creation failed. " << endl;
        WSACleanup();
        return 1;
    }
    //cout << "Socket created" << endl;.

    // Step 3: IP and port
    int port = 12345;
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; 
    
    serverAddr.sin_port = htons(port);
    //ip address to binaryy format
    if (InetPton(AF_INET, _T("0.0.0.0"), &serverAddr.sin_addr) != 1) {
        cout << " setting adress structure failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    //bind IP and port with socket
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr))) {
        cout << "bind failed " << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }


    //Step 4: Listen
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "listen failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "server has started listening on port: " << port << endl;
    vector<SOCKET> clients;


    //Step 5:Accept
    while (1) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cout << "invalid client socket " << endl;
        }
        clients.push_back(clientSocket);
        thread t1(InteractWithClient, clientSocket, std::ref(clients));
        t1.detach();
    }
    
    closesocket(serverSocket);
    logFile.close();
    return 0;
    
}
