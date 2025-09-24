#include <iostream>
#include <array>
#include <winsock2.h>

// #pragma comment(lib, "ws2_32.lib") // for MVSC.
// If you use mingw you need to add -lws2_32 flag when compile c++ code.

int main() {
    // Declaration data type
    int clientSize, bytesReceived;
    std::array<char, 1024> buffer;
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in server, client;

    clientSocket = INVALID_SOCKET;

    // WSAStartup initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(clientSocket);
        closesocket(serverSocket);
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

    // Configuration server address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9090); 

    // Bind socket to address and port
    if (bind(serverSocket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server ready. Wainting connection from port port 9090..." << std::endl;

    // Listen for input connection
    listen(serverSocket, SOMAXCONN);

    clientSize = sizeof(client);
    clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to get connection. Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected with client: " << inet_ntoa(client.sin_addr) << std::endl;

    // Recieved data from client
    bytesReceived = recv(clientSocket, buffer.data(), sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Message from client: " << buffer.data() << std::endl;
    } else {
        std::cerr << "Failed to get data. Error: " << WSAGetLastError() << std::endl;
    }

    // Close connection
    WSACleanup();
    closesocket(clientSocket);
    closesocket(serverSocket);
    return 0;
}