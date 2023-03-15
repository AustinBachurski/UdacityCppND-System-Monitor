#include <string>
#include "format.h"

std::string Format::ElapsedTime(long seconds)
{
    int32_t hrs = seconds / 3600;
    int32_t mins = (seconds - hrs * 3600) / 60;
    int32_t secs = seconds - (hrs * 3600 + mins * 60);
    return std::to_string(hrs) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
}