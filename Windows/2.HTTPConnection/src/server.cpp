#include <iostream>
#include <array>
#include <winsock2.h>

// #pragma comment(lib, "ws2_32.lib") // for MVSC.
// If you use mingw you need to add -lws2_32 flag when compile c++ code.

int main() {
    // Declaration data type
    int clientSize, bytesReceived;
    std::string response, content;
    std::array<char, 4096> buffer;
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in server, client;

    content = "<h1>Hello World!</h1>";
    response = "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: " + std::to_string(content.length()) + "\r\n"
               "\r\n" + content;
    clientSocket = INVALID_SOCKET;

    // WSAStartup initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(clientSocket);
        closesocket(serverSocket);
        return 1;
    }

    // Configuration server IP address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9090);

    // Bind socket
    if (bind(serverSocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server running in port 9090, Waiting connection..." << std::endl;

    // Listen
    listen(serverSocket, SOMAXCONN);
    clientSize = sizeof(client);

    while (true) {
        clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&client), &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to get connection. Error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        std::cout << "Connected with client: " << inet_ntoa(client.sin_addr) << std::endl;

        bytesReceived = recv(clientSocket, buffer.data(), buffer.size() - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            send(clientSocket, response.c_str(), response.length(), 0);
            std::cout << "request HTTP:\n\n" << buffer.data() << std::endl;
        } else {
            std::cerr << "Failed to get data. Error: " << WSAGetLastError() << std::endl;
        }

        // Close socket inside loop
        closesocket(clientSocket);
    }

    // Close socket
    WSACleanup();
    closesocket(serverSocket);
    return 0;
}