#define _WINSOCKAPI_
#define WIN32_LEAN_AND_MEAN
#include "GameWindow.h"
#include "InputSystem.h"
#include "GameWindow.h"
#include <thread>
#include <sstream>
#include <iostream>
#include <string>
#include <format>
#include <fstream>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define BUFLEN 2048  // max length of answer
#define PORT 42069  // the port on which to listen for incoming data
SOCKET client_socket;
sockaddr_in server;

std::string serverip;
bool temp = false;
bool connected = false;
GameWindow game;

//bool isServerAvailable(SOCKET client_socket, sockaddr_in& server)
//{
//    char pingMessage[] = "ping";
//    char recvBuffer[BUFLEN];
//
//     //Send a "ping" message to the server
//    if (sendto(client_socket, pingMessage, sizeof(pingMessage), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
//    {
//        printf("sendto() failed with error code: %d", WSAGetLastError());
//        isServerUp = false;
//        return false;
//    }
//
//    // Wait for a response from the server
//    int serverAddrLength = sizeof(server);
//    int recvLen = recvfrom(client_socket, recvBuffer, BUFLEN, 0, (sockaddr*)&server, &serverAddrLength);
//    if (recvLen == SOCKET_ERROR)
//    {
//        // Error occurred while receiving
//        printf("recvfrom() failed with error code: %d", WSAGetLastError());
//        isServerUp = false;
//        return false;
//    }
//
//    // Check the received message to determine server availability
//    if (std::string(recvBuffer, recvBuffer + recvLen) == "pong")
//    {
//        isServerUp = true;  // Server is available
//        return true;
//    }
//    else
//    {
//        isServerUp = false;  // Server is not available
//        return false;
//    }
//}

std::vector<std::string> splitString(std::string str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, '$')) {
        tokens.push_back(token);
    }

    return tokens;
}

int send() {
    while (1)
    {

    }
}

int recieve() {
    while(1){
        char recvBuffer[BUFLEN];
        int serverAddrLength = sizeof(server);
        int recvLen = recvfrom(client_socket, recvBuffer, BUFLEN, 0, (sockaddr*)&server, &serverAddrLength);
        printf("here!");
        std::string decodedData(recvBuffer, recvBuffer + recvLen);
        const char* myCharPointer = decodedData.c_str();
        // Print the decoded data
        printf(myCharPointer);
        if (recvLen == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code: %d", WSAGetLastError());
            return 2;
        }
            
        // Check the received message to determine server availability
        std::string data = std::string(recvBuffer, recvBuffer + recvLen);
        std::vector<std::string> splitdata = splitString(data);
        std::cout << "\n" << splitdata[1] << std::endl;
        if (data[0] == 'M')
        {
            //enemyPos = Vector3D(std::stoi(splitdata[1]), std::stoi(splitdata[2]), std::stoi(splitdata[3]));
            game.setEnemyPos(std::stoi(splitdata[1]), std::stoi(splitdata[2]), std::stoi(splitdata[3]));
        }
        else if (data[0] == 'D')
            connected = false;
        else if (data[0] == 'S')
            if (splitdata[1] == "HIT") {
                game.setEnemyPos(1000, 0, 0);
            }
            else if (splitdata[1] == "DEAD") {
                game.new_pos = Vector3D(1000, 0, 0);
            }
    }
}

int login() {
    // Start communication
    while (!connected)
    {
        char message[BUFLEN];
        printf("Loging In\n");
        std::string mess = "L$";
        strcpy_s(message, mess.c_str());

        // Send the message
        if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code: %d", WSAGetLastError());
            return 1;
        }

        // Wait for a response from the server
        char recvBuffer[BUFLEN];
        int serverAddrLength = sizeof(server);
        int recvLen = recvfrom(client_socket, recvBuffer, BUFLEN, 0, (sockaddr*)&server, &serverAddrLength);
        if (recvLen == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code: %d", WSAGetLastError());
            return 2;
        }

        // Check the received message to determine server availability
        if (std::string(recvBuffer, recvBuffer + recvLen) == "L$Connected!")
        {
            printf("Connected to the server.\n");
            connected = 1;
        }
        else
        {
            printf("Connection failed. Retrying...\n");
            Sleep(1000); // Wait for 1 second before retrying
        }
    }

    return 0;
}

int main() {
    std::ifstream inputFile("config.txt");
    std::string line;
    std::getline(inputFile, line);
    const char* serverip = line.c_str();
    system("title UDP Client");

    // Initialize Winsock
    WSADATA ws;
    printf("Initialising Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket == INVALID_SOCKET)
    {
        printf("socket() failed with error code: %d", WSAGetLastError());
        return 2;
    }

    // Setup address structure
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(serverip);

    login();
    std::thread thread(recieve);
    thread.detach();
    while (1) { 
        if (connected)
        {
            try
            {
                GraphicsEngine::create();
                InputSystem::create();
            }
            catch (...) { return -1; }
            {
                try {
                    while (game.isRunning())
                    {
                        // Attempt to send a message to the serve
                        int x = game.new_pos.m_x;
                        int y = game.new_pos.m_y;
                        int z = game.new_pos.m_z;

                        std::string playerDirPacketS = "M$" + std::to_string(z) + '$' + std::to_string(y) + '$' + std::to_string(x);
                        char playerDirPacket[] = "";
                        std::strcpy(playerDirPacket, playerDirPacketS.c_str());
                        std::cout << playerDirPacket << std::endl;
                        if (sendto(client_socket, playerDirPacket, strlen(playerDirPacket), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
                        {
                            printf("sendto() failed with error code: %d", WSAGetLastError());
                            return 3;
                        }
                        if (game.hit) {
                            std::string projPacketS = "S$";
                            char projPacket[] = "";
                            std::strcpy(projPacket, projPacketS.c_str());
                            std::cout << projPacket << std::endl;
                            game.hit = false;
                            if (sendto(client_socket, projPacket, strlen(projPacket), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
                            {
                                printf("sendto() failed with error code: %d", WSAGetLastError());
                                return 3;
                            }
                        }
                    }
                }
                catch (...) {

                    InputSystem::release();
                    GraphicsEngine::release();
                    return -1;
                }
            }
            closesocket(client_socket);
            WSACleanup();
            InputSystem::release();
            GraphicsEngine::release();
            return 0;
        }
    }
}