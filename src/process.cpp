#include <unistd.h>
#include <string>
#include "process.h"

Process::Process(
    long int age,
    std::string command,
    float cpuUtilization,
    std::string memory, 
    int pid, 
    std::string user
    ):
m_age{ age },
m_command{ command },
m_cpuUtilization{ cpuUtilization },
m_memory{ memory },
m_pid{ pid },
m_user{ user }
{}

int Process::Pid()
{
    return m_pid;
}

float Process::CpuUtilization()
{
    return m_cpuUtilization;
}

std::string Process::Command()
{
    return m_command;
}

std::string Process::Ram()
{
    return m_memory;
}

std::string Process::User()
{
    return m_user;
}

long int Process::UpTime()
{
    return m_age;
}

bool Process::operator<(const Process& comparison) const
{
    // Sort by highest CPU usage
    // return m_cpuUtilization > comparison.m_cpuUtilization;
    
    // Sort by highest Memory usage
    float a = std::stof(m_memory);
    float b = std::stof(comparison.m_memory);
    return a > b;
}