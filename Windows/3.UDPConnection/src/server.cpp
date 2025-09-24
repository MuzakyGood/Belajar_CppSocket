#include <iostream>
#include <array>
#include <winsock2.h>

// #pragma comment(lib, "ws2_32.lib") // for MVSC.
// If you use mingw you need to add -lws2_32 flag when compile c++ code.

int main() {
    int clientAddrSize, byteReceived;
    std::string reply;
    std::array<char, 1024> buffer;
    WSADATA wsa;
    SOCKET udpSocket;
    sockaddr_in serverSocket, clientSocket;

    clientAddrSize = sizeof(clientSocket);
    reply = "Message received!\n";
    udpSocket = INVALID_SOCKET;

    // Initialization WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(udpSocket);
        return -1;
    }

    // Create UDP socket 
    udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create UDP socket!" << std::endl;
        WSACleanup();
        closesocket(udpSocket);
        return -1;
    }

    // Configuration server address
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(9090);
    serverSocket.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(udpSocket, reinterpret_cast<SOCKADDR*>(&serverSocket), sizeof(serverSocket)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(udpSocket);
        return -1;
    }

    std::cout << "UDP Server active in port 9090..." << std::endl;

    while (true) {
        byteReceived = recvfrom(udpSocket, buffer.data(), buffer.size() - 1, 0, reinterpret_cast<SOCKADDR*>(&clientSocket), &clientAddrSize);
        if (byteReceived == SOCKET_ERROR) {
            std::cerr << "Receive failed" << std::endl;
            continue;
        }

        buffer[byteReceived] = '\0';
        std::cout << "Message from client: " << buffer.data() << std::endl;

        sendto(udpSocket, reply.c_str(), reply.length(), 0, reinterpret_cast<SOCKADDR*>(&clientSocket), clientAddrSize);
    }

    WSACleanup();
    closesocket(udpSocket);
    return 0;
}