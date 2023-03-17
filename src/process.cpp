#include <unistd.h>
#include <string>

#include "process.h"

Process::Process(
    int pid, 
    float utilization, 
    std::string command, 
    std::string memory, 
    std::string user, 
    long int age
):
m_pid{ pid },
m_cpuUtilization{ utilization },
m_generatedCommand{ command },
m_memoryUtilization{ memory },
m_user{ user },
m_age{ age }
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
    return m_generatedCommand;
}

std::string Process::Ram()
{
    return m_memoryUtilization;
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
    return m_cpuUtilization < comparison.m_cpuUtilization;
}