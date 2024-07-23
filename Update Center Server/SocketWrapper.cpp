#include "SocketWrapper.h"

Socket::Socket(const std::string servAddress, const int servPort)
{
    this->serverAddressLiteral = servAddress;
    this->serverPort = servPort;

    socketStatus = true;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        socketStatus = false;
        throw ServerException(WSAGetLastError(), "Winsock initialization error");
    }
    
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        socketStatus = false;
        throw ServerException(WSAGetLastError(), "Invalid Winsock version");
    }
    
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socketId = serverSocket;
    if (socketId == INVALID_SOCKET)
    {
        socketStatus = false;
        throw ServerException(WSAGetLastError(), "Socket creation error");
    }

    ZeroMemory(&serverAddressStruct, sizeof(serverAddressStruct));
    serverAddressStruct.sin_family = AF_INET;
    serverAddressStruct.sin_port = htons(servPort);
    //auto servAddrLitTemp = serverAddressLiteral.c_str();
    //serverAddressStruct.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddressStruct, sizeof(serverAddressStruct)) == SOCKET_ERROR)
    {
        socketStatus = false;
        throw ServerException(WSAGetLastError(), "Socket configuiring error");
    }

    std::cout << "Server Socket initialization was successful." << std::endl;
}

Socket::Socket()
{
    this->serverAddressLiteral = "127.0.0.1"; //standard server parameters
    this->serverPort = 8080;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        socketStatus = false;
        throw ServerException(WSAGetLastError(), "Winsock initialization error");
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        socketStatus = false;
        throw ServerException(WSAGetLastError(), "Incorrect Winsock version");
    }

    serverSocket = NULL;
    socketId = NULL;

    ZeroMemory(&serverAddressStruct, sizeof(serverAddressStruct));
    
    socketStatus = false;
}

Socket::~Socket()
{
    closesocket(socketId);
    WSACleanup();
    socketStatus = false;
}

int Socket::GetSocketId()
{
    return socketId;
}

bool Socket::GetSocketStatus()
{
    return socketStatus;
}

std::string Socket::GetServerAddress()
{
    return serverAddressLiteral;
}

int Socket::GetServerPort()
{
    return serverPort;
}

sockaddr_in Socket::GetServerAddressStruct()
{
    return serverAddressStruct;
}

int Socket::SendFile(std::string filePath, char* buffer)
{
    std::ifstream file(filePath, std::ios::binary);

    int bytesRead;
    while (!file.eof()) 
    {
        file.read(buffer, sizeof(buffer));
        bytesRead = static_cast<int>(file.gcount());

        if (bytesRead > 0) 
        {
            int bytesSent = send(serverSocket, buffer, bytesRead, 0);
            if (bytesSent == SOCKET_ERROR) 
            {
                file.close();
                throw std::ifstream::failure("send failed");
                return bytesSent;
            }
        }
    }
}

void Socket::ReceiveRequest(SOCKET clientSocket, char* buffer)
{
    if (recv(clientSocket, buffer, sizeof(buffer), 0) == SOCKET_ERROR)
        throw ServerException(SendReceiveError, "Listening request from client error");
}

void Socket::SendRequestAndReceiveResponse(SOCKET clientSocket, std::string request, char* buffer)
{
    int bytesRead;

    int sendStatus = send(clientSocket, request.c_str(), request.length(), 0);
    if (sendStatus == SOCKET_ERROR)
    {
        std::string errorMessage = "Sending request (" + request;
        errorMessage += ") error";
        throw ServerException(SendReceiveError, errorMessage);
    }    

    bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == SOCKET_ERROR)
        throw ServerException(SendReceiveError, "Response receiving error");

}
