/////////////////////////////////////////////////////////////////////////////
//Update Center Server, v.1.0
/////////////////////////////////////////////////////////////////////////////
//Copyright (c) Kirill Belozerov, 2021-2023. All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Notes: 
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define _CRT_SECURE_NO_WARNINGS

//Information about this Program Build. 
#define VERSION "Version 1.0"
#define AUTHOR "Copyright (c) Kirill Belozerov, 2021-2023. All Rights Reserved"

#define UNDERCONSTR "Under construction"
#define DEBUG 1.0


const string FILE_DIRECTORY = "files/"; // ���������� � �������

int main() 
{
    setlocale(LC_ALL, "Rus");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        cerr << "������ ������������� winsock" << endl;
        return 1;
    }

    // �������� ������
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) 
    {
        cerr << "������ �������� ������" << endl;
        WSACleanup();
        return 1;
    }

    // �������� ������ � ����� � ������
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // ���� �������
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) 
    {
        cerr << "������ �������� ������" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // ������������� �������� ����������
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) 
    {
        cerr << "������ ������������� �������� ����������" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "������ ������� � ������� �����������..." << endl;

    while (true) {
        // �������� �������
        sockaddr_in clientAddress{};
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) 
        {
            cerr << "������ ����������� �������" << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // ������ ������� �� �������
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) 
        {
            cerr << "������ ������ ������� �� �������" << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        // ���������� ����� ����� �� �������
        string filename(buffer);
        filename = filename.substr(0, filename.find('\r'));

        // �������� �����
        ifstream file(FILE_DIRECTORY + filename, ios::binary);
        if (!file) {
            cerr << "������ �������� �����: " << filename << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        // ������ ����������� �����
        ostringstream fileContentStream;
        fileContentStream << file.rdbuf();
        string fileContent = fileContentStream.str();

        // �������� ����������� ����� �������
        if (send(clientSocket, fileContent.c_str(), fileContent.length(), 0) == SOCKET_ERROR) 
        {
            cerr << "������ �������� ����������� ����� �������" << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        cout << "���� ������� ��������� �������: " << filename << endl;

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
