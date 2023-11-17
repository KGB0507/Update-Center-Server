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


const string FILE_DIRECTORY = "files/"; // Директория с файлами

int main() 
{
    setlocale(LC_ALL, "Rus");

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        cerr << "Ошибка инициализации winsock" << endl;
        return 1;
    }

    // Создание сокета
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) 
    {
        cerr << "Ошибка создания сокета" << endl;
        WSACleanup();
        return 1;
    }

    // Привязка сокета к порту и адресу
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); // Порт сервера
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, reinterpret_cast<const sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) 
    {
        cerr << "Ошибка привязки сокета" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Прослушивание входящих соединений
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) 
    {
        cerr << "Ошибка прослушивания входящих соединений" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Сервер запущен и ожидает подключений..." << endl;

    while (true) {
        // Ожидание клиента
        sockaddr_in clientAddress{};
        int clientAddressSize = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
        if (clientSocket == INVALID_SOCKET) 
        {
            cerr << "Ошибка подключения клиента" << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        // Чтение запроса от клиента
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR) 
        {
            cerr << "Ошибка чтения запроса от клиента" << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        // Извлечение имени файла из запроса
        string filename(buffer);
        filename = filename.substr(0, filename.find('\r'));

        // Открытие файла
        ifstream file(FILE_DIRECTORY + filename, ios::binary);
        if (!file) {
            cerr << "Ошибка открытия файла: " << filename << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        // Чтение содержимого файла
        ostringstream fileContentStream;
        fileContentStream << file.rdbuf();
        string fileContent = fileContentStream.str();

        // Отправка содержимого файла клиенту
        if (send(clientSocket, fileContent.c_str(), fileContent.length(), 0) == SOCKET_ERROR) 
        {
            cerr << "Ошибка отправки содержимого файла клиенту" << endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        cout << "Файл успешно отправлен клиенту: " << filename << endl;

        closesocket(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
