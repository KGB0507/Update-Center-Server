#include "RequestSubroutines.h"

std::vector<std::string> SplitRequest(const std::string& input, const std::string& delimiter)
{
    std::vector<std::string> result;
    size_t pos = 0;
    size_t delimiterPos = input.find('\t');

    while (delimiterPos != std::string::npos) 
    {
        result.push_back(input.substr(pos, delimiterPos - pos));
        pos = delimiterPos + 1;
        delimiterPos = input.find('\t', pos);
    }

    result.push_back(input.substr(pos));

    return result;
}

int StringToInt(const std::string& str)
{
    int result = 0;
    bool isNegative = false;
    std::string strInternal = str;

    if (strInternal.front() == '-')
    {
        isNegative = true;
        strInternal.erase(0, 1);
    }

    for (char c : strInternal)
    {
        if (c >= '0' && c <= '9') 
        {
            result = result * 10 + (c - '0');
        }
        else 
        {
            return 0;
        }
    }

    if (isNegative) 
    {
        result = -result;
    }

    return result;
}
