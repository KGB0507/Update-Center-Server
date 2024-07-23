/////////////////////////////////////////////////////////////////////////////
//Update Center Server, v.1.0
/////////////////////////////////////////////////////////////////////////////
//Copyright (c) Kirill Belozerov, 2021-2024. All Rights Reserved
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//Notes: 
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <Windows.h>
#include <atomic>
#include <condition_variable>
#include "SocketWrapper.h"
#include "ConnectionManager.h"
#include "Settings.h"
#include "ServerException.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#define _CRT_SECURE_NO_WARNINGS

//Information about this Program Build. 
#define VERSION "Version 1.0"
#define AUTHOR "Copyright (c) Kirill Belozerov, 2021-2024. All Rights Reserved."

//#define DEBUG 1.0

#ifdef DEBUG
#define UNDERCONSTR "Under construction"
#endif

#define STARTUPSNDPATH L"play ./Resource/sound/startup.mp3 wait"

std::atomic_bool running(true);
std::condition_variable cv;

void StopApplication() 
{
    //std::cout << "Press Ctrl+Z to stop the application..." << std::endl;
    while (running) 
    {
        if (std::cin.get() == '\3') 
        { 
            running = false;
            cv.notify_all();
            std::cout << "Stopping application..." << std::endl;
            break;
        }
    }
}

int main() 
{
    //setlocale(LC_ALL, "Rus");

    std::cout << "Update Center Server " << VERSION << std::endl;
    std::cout << AUTHOR << std::endl;
#ifdef DEBUG
    std::cout << UNDERCONSTR << std::endl << std::endl;
#endif

#ifndef DEBUG
    mciSendString(STARTUPSNDPATH, NULL, 0, NULL); //playing startup sound
#endif

    std::cout << std::endl << std::endl;

    int status = 0;

    try
    {
        Settings settings;

        std::string serverAddress = settings.GetServerAddr();
        int serverPort = settings.GetServerPort();
        std::string distributivesLocation = settings.GetDistributivesLocation();

        Socket serverSocket(serverAddress, serverPort);

        ConnectionManager connectionManager(distributivesLocation);

        std::thread stopper(StopApplication);
        ////////
        status = connectionManager.ManageIncomingConnections(serverSocket);
        ////////
    }
    catch(ServerException ex)
    {
        std::cout << "Server error: " << ex.What() << std::endl;
    }
    catch (std::fstream::failure ex)
    {
        std::cout << "Server error: " << ex.what() << std::endl << ex.code() << std::endl;
    }
    catch (std::exception ex)
    {
        std::cout << "Server error: " << ex.what() << std::endl;
    }

    std::cout << "Server stopped. Status = " << status << std::endl;

    return 0;
}
