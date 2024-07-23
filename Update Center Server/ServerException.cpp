#include "ServerException.h"

ServerException::ServerException(const int errorCode, const std::string errorMessage)
{
	this->errorCode = errorCode;
	this->errorMessage = errorMessage;
}

ServerException::ServerException(const std::string errorMessage)
{
	this->errorCode = 99; //unknown error code 
	this->errorMessage = errorMessage;
}

std::string ServerException::What()
{
	std::string fullErrorMessage  = errorMessage + ": ";
	return fullErrorMessage += std::to_string(errorCode);
}
