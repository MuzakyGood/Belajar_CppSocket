#include <iostream>
#include <array>
#include <winsock2.h>

// #pragma comment(lib, "ws2_32.lib") // for MVSC.
// If you use mingw you need to add -lws2_32 flag when compile c++ code.

int main() {
    // Declaration data type
    int serverAddrSize, bytesReceived;
    std::string message;
    std::array<char, 1024> buffer;
    WSADATA wsa;
    SOCKET udpClient;
    sockaddr_in serverSocket;

    serverAddrSize = sizeof(serverSocket);
    message = "Hello from client UDP\n";
    udpClient = INVALID_SOCKET;

    // Initialization WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(udpClient);
        return -1;
    }

    // Create UDP socket 
    udpClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpClient == INVALID_SOCKET) {
        std::cerr << "Failed to create UDP socket!" << std::endl;
        WSACleanup();
        closesocket(udpClient);
        return -1;
    }

    // Configuration IP address server
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_port = htons(9090);
    serverSocket.sin_addr.s_addr = inet_addr("127.0.0.1");

    sendto(udpClient, message.c_str(), message.length(), 0, reinterpret_cast<SOCKADDR*>(&serverSocket), serverAddrSize);

    std::cout << "Message send to server." << std::endl;

    // Get reply from server
    bytesReceived = recvfrom(udpClient, buffer.data(), buffer.size() - 1, 0, reinterpret_cast<SOCKADDR*>(&serverSocket), &serverAddrSize);
    if (bytesReceived == SOCKET_ERROR) {
        std::cerr << "Received from failed!" << std::endl;
    } else {
        buffer[bytesReceived] = '\0';
        std::cout << "Reply from server: " << buffer.data() << std::endl;
    }

    WSACleanup();
    closesocket(udpClient);
    return 0;
}