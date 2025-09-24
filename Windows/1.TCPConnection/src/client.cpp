#include <iostream>
#include <winsock2.h>

// #pragma comment(lib, "ws2_32.lib") // for MVSC.
// If you use mingw you need to add -lws2_32 flag when compile c++ code.

int main() {
    // Declaration data type
    std::string message;
    WSADATA wsa;
    SOCKET clientSocket;
    sockaddr_in server;

    message = "Hello, World, from client.";
    clientSocket = INVALID_SOCKET;

    // Winsock initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(clientSocket);
        return -1;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        closesocket(clientSocket);
        return -1;
    }

    // Configuration address server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(9090);

    // Connect to server
    if (connect(clientSocket, reinterpret_cast<const sockaddr *>(&server), sizeof(server)) < 0) {
        std::cerr << "Failed to connect server. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(clientSocket);
        return -1;
    }

    std::cout << "Successfull connect to server" << std::endl;

    // Send data to server
    if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Failed send data to server. Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Message successfull sended to server" << std::endl;
    }

    // Close socket
    WSACleanup();
    closesocket(clientSocket);
    return 0;
}