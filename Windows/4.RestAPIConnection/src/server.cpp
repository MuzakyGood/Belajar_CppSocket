#include <iostream>
#include <array>
#include <winsock2.h>

// #pragma comment(lib, "ws2_32.lib") // for MVSC.
// If you use mingw you need to add -lws2_32 flag when compile c++ code.

int main() {
    int clientData, bytesReceived;
    size_t padStart, padEnd;
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    sockaddr_in server, client;
    std::string response, content, path;
    std::array<char, 1024> buffer;

    clientSocket = INVALID_SOCKET;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Invalid startup error: " << WSAGetLastError() << std::endl;
        return -1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
        closesocket(clientSocket);
        closesocket(serverSocket);
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(9090);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&server), sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        closesocket(clientSocket);
        closesocket(serverSocket);
        return -1;
    }

    std::cout << "Server running in port 9090, Waiting connection..." << std::endl;

    listen(serverSocket, 0);
    clientData = sizeof(client);

    while (true) {
        clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&client), &clientData);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to get connection. Error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            closesocket(clientSocket);
            closesocket(serverSocket);
            return 1;
        }

        bytesReceived = recv(clientSocket, buffer.data(), buffer.size() - 1, 0);
        if (bytesReceived < 0) {
            std::cerr << "Failed to get data. Error: " << WSAGetLastError() << std::endl;
        }

        std::string request(buffer.data());

        padStart = request.find('/');
        if (padStart != std::string::npos) {
            padEnd = request.find(' ', padStart);
            path = request.substr(padStart, padEnd - padStart);
        }

        // Route dan Request harus ada di atas
        if ((request.rfind("GET", 0) == 0) && path == "/homepage") {
            buffer[bytesReceived] = '\0';

            content = "{\n"
                      "    \"respond\": \"Request GET and route /homepage detected.\"\n"
                      "}";
            response = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(content.length()) + "\r\n"
               "\r\n" + content;

            send(clientSocket, response.c_str(), response.length(), 0);

        // Request saja boleh di bawah
        } else if (request.rfind("GET", 0) == 0) {
            buffer[bytesReceived] = '\0';

            content = "{\n"
              "    \"respond\": \"Request GET detected.\"\n"
              "}";
            response = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(content.length()) + "\r\n"
               "\r\n" + content;

            send(clientSocket, response.c_str(), response.length(), 0);

        } else if (request.rfind("POST") == 0) {
            buffer[bytesReceived] = '\0';

            content = "{\n"
              "    \"respond\": \"Request POST detected.\"\n"
              "}";
            response = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(content.length()) + "\r\n"
               "\r\n" + content;

            send(clientSocket, response.c_str(), response.length(), 0);
        } else if (request.rfind("PUT", 0) == 0) { 
            buffer[bytesReceived] = '\0';

            content = "{\n"
              "    \"respond\": \"Request PUT detected.\"\n"
              "}";
            response = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(content.length()) + "\r\n"
               "\r\n" + content;

            send(clientSocket, response.c_str(), response.length(), 0);
        } else if (request.rfind("DELETE", 0) == 0) { 
            buffer[bytesReceived] = '\0';

            content = "{\n"
              "    \"respond\": \"Request DELETE detected.\"\n"
              "}";
            response = "HTTP/1.1 200 OK\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(content.length()) + "\r\n"
               "\r\n" + content;

            send(clientSocket, response.c_str(), response.length(), 0);
        } else {
            buffer[bytesReceived] = '\0';

            content = "{\n"
              "    \"respond\": \"Request available: GET, POST, PUT, DELETE!\"\n"
              "}";
            response = "HTTP/1.1 400 Request Not Allowed\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: " + std::to_string(content.length()) + "\r\n"
               "\r\n" + content;

            send(clientSocket, response.c_str(), response.length(), 0);
        }

        closesocket(clientSocket);
    }

    WSACleanup();
    closesocket(serverSocket);
    return 0;
}