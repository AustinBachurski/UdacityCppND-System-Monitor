#include <string>
#include "format.h"

std::string Format::ElapsedTime(long seconds)
{
    long hrs = seconds / 3600;
    long mins = (seconds - hrs * 3600) / 60;
    long secs = seconds - (hrs * 3600 + mins * 60);

    return std::to_string(hrs) + ":" + std::to_string(mins) + ":" + std::to_string(secs);
}