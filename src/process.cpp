#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"


int Process::Pid()
{
    // TODO: Return this process's ID
}

float Process::CpuUtilization()
{
    // TODO: Return this process's CPU utilization
}

std::string Process::Command()
{
    // TODO: Return the command that generated this process
}

std::string Process::Ram()
{
    // TODO: Return this process's memory utilization
}

std::string Process::User()
{
    // TODO: Return the user (name) that generated this process
}

long int Process::UpTime()
{
    // TODO: Return the age of this process (in seconds)
}

bool Process::operator<(Process const& a[[maybe_unused]]) const
{
    // TODO: Overload the "less than" comparison operator for Process objects
    // REMOVE: [[maybe_unused]] once you define the function
}