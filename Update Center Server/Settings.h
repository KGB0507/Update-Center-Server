#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include "ServerException.h"

#define SETTINGSFILEPATH "./config/settings.cfg"
#define NUMOFPROPERTIES 2

class Settings
{
private:
	std::fstream settingsFile;

	std::string serverAddr;
	int serverPort;

	std::string distributivesLocation;

	std::string keyStopCombination;

public:
	Settings();

	~Settings();

	std::string GetServerAddr();

	int GetServerPort();

	std::string GetDistributivesLocation();

	std::string GetKeyStopCombination();
};
