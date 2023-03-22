#include <string>
#include "format.h"

std::string Format::ElapsedTime(long seconds)
{
    int hrs = seconds / 3600;
    int mins = (seconds % 3600) / 60;
    int secs = (seconds % 3600) % 60;
    
    std::string hrsPrefix = hrs < 10 ? "0" : "";
    std::string minsPrefix = mins < 10 ? "0" : "";
    std::string secsPrefix = secs < 10 ? "0" : "";

    return hrsPrefix + std::to_string(hrs) 
            + ":" + minsPrefix + std::to_string(mins) 
            + ":" + secsPrefix + std::to_string(secs);
}