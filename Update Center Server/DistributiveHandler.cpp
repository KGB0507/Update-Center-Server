#include "DistributiveHandler.h"

std::string PathCombine(std::string path1, std::string path2)
{
    if (path1.back() == '/') 
    {
        path1.pop_back();
    }

    if (path2.front() != '/') 
    {
        path2 = "/" + path2;
    }

    return path1 + path2;
}

ServerResponse InspectUpdates(std::string distributivesLocation, int appNumber, std::string currentVersion)
{
    if (appNumber < 0)
        throw new ServerException(ClientRequestError, "Invalid application ID requested");

    ServerResponse serverResponse;

    std::vector<int> numbers;
    std::ifstream file(PathCombine(distributivesLocation, "app.spec"));
    std::string line;

    if (!file.is_open())
        throw new ServerException("File 'app.spec' opening error");

    while (std::getline(file, line)) 
    {
        if (line.substr(0, 2) != "//") 
        {
            int num = std::stoi(line);
        }
    }
    file.close();

    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return serverResponse;
}

/*
ServerResponse DistributiveHandler::InspectUpdates()
{
    return ServerResponse();
}
*/