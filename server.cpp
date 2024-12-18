#include <iostream>
#include <winsock2.h> 
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>

#pragma comment(lib, "ws2_32.lib")  // Link the Winsock library
using namespace std;


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
    WSADATA wsaData; // Struct stores Winsock details
    //Socket- "endpoint" through which data is sent and received.
    
    // Step 1: Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }
    //cout << "Winsock initialized successfully!" << endl;

    // Step 2: Create a socket
    //SOCKET is a data type in Winsock that represents a socket.
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP Socket
    // AF_INET (Address Family)--- tells the socket to use the IPv4 protocol(the most common network protocol).
    //SOCK_STREAM              ---It specifies that this is a TCP socket(used for reliable communication).
    //                            TCP ensures data is delivered correctly and in the right order.
    //                            If you wanted to use UDP(faster but less reliable), you would use SOCK_DGRAM
    if (serverSocket == INVALID_SOCKET) {
        cout << "Socket creation failed. " << endl;
        WSACleanup();
        return 1;
    }
    //cout << "Socket created" << endl;
    //The serverSocket variable now holds the "telephone number" (descriptor) of the socket.
    //You can start using this socket to send / receive data or wait for incoming connections.

    // Step 3: IP and port
    int port = 12345;
    sockaddr_in serverAddr; // form where we fill in details like IP address, port number, and protocol.
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; //accept connections from any IP address.
                                             //If the server has multiple network interfaces(e.g., Ethernet and Wi - Fi), 
                                             // this allows it to listen on all of them.
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

    
}
