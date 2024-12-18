#include <iostream>
#include<string>
#include <winsock2.h> 
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void SendMsg(SOCKET s) {
    cout << "enter your chat name:" << endl;
    string name;
    getline(cin, name);
    string message;

    while (1) {
        getline(cin, message);
        string msg = name + " : " + message;
        int bytesent = send(s, msg.c_str(), msg.length(), 0);
        if (bytesent == SOCKET_ERROR) {
            cout << " error sending message" << endl;
            break;
        }
        if (message == "quit") {
            cout << "chat done" << endl;

        }
    }

    closesocket(s);
    WSACleanup();
}


void ReceiveMsg(SOCKET s) {
    char buffer[4096];
    int recvlength;
    string msg = "";
    while (1){
        recvlength = recv(s, buffer, sizeof(buffer), 0);
        if (recvlength <= 0) {
            cout << "disconnected from server" << endl;
            break;
        }
        else {
            msg = string(buffer, recvlength);
            cout << msg << endl;
        }
    }
    closesocket(s);
    WSACleanup();
}

int main() {
    WSADATA wsaData; // Struct stores Winsock details

    // Step 1: Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "WSAStartup failed!" << endl;
        return 1;
    }
    cout << "Winsock initialized successfully!" << endl;

    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "invalid socket. " << endl;
        WSACleanup();
        return 1;
    }
    int port = 12345;
    string serveraddress = "127.0.0.1";

    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));


    if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        cout << "couldn't connect to server " << endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }
    cout << "connnected to server" << endl;

    thread senderthread(SendMsg, s);
    thread receiverthread(ReceiveMsg, s);
    
    senderthread.join();
    receiverthread.join();


    return 0;
}
