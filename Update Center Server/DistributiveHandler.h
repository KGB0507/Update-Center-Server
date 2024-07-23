#pragma once

#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include "ServerException.h"

struct UpdateInfo
{
    std::string version;
    int priority = 0;
    std::string releaseDate;
    double sizeInKBytes = 0;
};

enum ResponseStatus
{
    OK = 200,
    NOUPDATES = 300,
    NOTFOUND = 404
};

struct ServerResponse
{
    int status;
    int numOfUpdates;
    std::list<UpdateInfo> availableUpdateList;
};

std::string PathCombine(std::string path1, std::string path2);

ServerResponse InspectUpdates(std::string distributivesLocation, int appNumber, std::string currentVersion);
/*
class DistributiveHandler
{
private:
    std::string distributivesLocation;

public:
    ServerResponse InspectUpdates();
};
*/
