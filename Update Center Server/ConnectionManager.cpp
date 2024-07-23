#include "ConnectionManager.h"


ConnectionManager::ConnectionManager()
{
    this->distributivesLocation = "./Resource/Distrib/";

    numOfConnections = 0;

    std::cout << "Connection Manager configuiring was successful." << std::endl;
}

ConnectionManager::ConnectionManager(std::string distributivesLocation)
{
    this->distributivesLocation = distributivesLocation;

    numOfConnections = 0;

    std::cout << "Connection Manager configuiring was successful." << std::endl;
}

ConnectionManager::~ConnectionManager()
{
    for (auto& thread : clientThreads)
    {
        thread.join();
    }

    clientThreads.clear();
}

int ConnectionManager::ManageIncomingConnections(Socket serverSocket)
{
    int status = 0;
    SOCKET serverSocketId = serverSocket.GetSocketId();
    std::string serverAddress = serverSocket.GetServerAddress();
    int serverPort = serverSocket.GetServerPort();

    std::cout << "Server configuiring was completed." << std::endl;
    std::cout << "Server Address: " << serverAddress << std::endl;
    std::cout << "Server Port: " << serverPort << std::endl << std::endl;

    while (status == 0)
    {
        std::cout << "Client connections listening is in progress..." << std::endl;

        if (listen(serverSocketId, SOMAXCONN) == SOCKET_ERROR)
            throw ServerException(ListenSocketError, "Listening incoming connections error");

        ClientInfo clientInfo;

        ZeroMemory(&clientInfo.clientAddress, sizeof(clientInfo.clientAddress));
        clientInfo.clientAddressSize = sizeof(clientInfo.clientAddress);
        clientInfo.clientSocket = accept(serverSocketId, reinterpret_cast<sockaddr*>(&clientInfo.clientAddress), &clientInfo.clientAddressSize);
        if (clientInfo.clientSocket == INVALID_SOCKET)
            throw ServerException(CreateSocketError, "Client socket creation error");
        else
        {
            char* ClientIP = inet_ntoa(clientInfo.clientAddress.sin_addr);
            int ClientPort = ntohs(clientInfo.clientAddress.sin_port);
            std::cout << "Client connected with IP " << ClientIP << ClientPort << std::endl;
        }

        //inet_ntop(AF_INET, &clientInfo.clientAddress.sin_addr, clientInfo.clientIP, INET_ADDRSTRLEN);	// Convert connected client's IP to standard string format
        
        numOfConnections++;
        //std::cout << "Client " << numOfConnections << " connected with IP address " << clientInfo.clientIP << std::endl;
        
        // —оздание нового потока дл€ обработки клиента
        std::thread clientThread([this, &serverSocket, &clientInfo]()
        {
            this->HandleConnection(serverSocket, clientInfo);
        });
        clientThreads.push_back(std::move(clientThread));
    }

    return status;
}

int ConnectionManager::GetNumOfConnections()
{
    return numOfConnections;
}

int ConnectionManager::HandleConnection(Socket serverSocket, ClientInfo clientInfo)
{
    int status = 0;

    char* buffer = InitBuffer(STDBUFFERSIZE);
    clientInfo.buffer = buffer;
    clientInfo.bufSize = sizeof(clientInfo.buffer);

    serverSocket.ReceiveRequest(clientInfo.clientSocket, buffer);

    std::string clientRequest(buffer);

    std::vector<std::string> clientRequestSplitted = SplitRequest(clientRequest, "\t");

    int appNumber = StringToInt(clientRequestSplitted[1]);
    std::string version = clientRequestSplitted[2];

    ServerResponse serverResponse = InspectUpdates(distributivesLocation, appNumber, version);

    //////////////////////////////
    // 
    //////////////////////////////

    numOfConnections--;

    return status;
}

char* InitBuffer(const int bufferSize)
{
    return new char(bufferSize);
}

void KillBuffer(char* buffer)
{
    delete[] buffer;
    buffer = nullptr;
}

