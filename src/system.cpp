#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
/*
Kernel and Operating System information won't change while the tool is
open - retrieve the information once and store in member variables.
*/
System::System():
m_cpu{},
m_processes{},
m_kernel{},
m_operatingSystem{}
{
    m_kernel = LinuxParser::Kernel();
    m_operatingSystem = LinuxParser::OperatingSystem();
}

Processor& System::Cpu()
{
    return m_cpu;
}

std::vector<Process>& System::Processes()
{ 
    for (const int& pid : *LinuxParser::Pids())
    {
        



        m_processes.push_back(Process());  // Think about emplace, understand it.
    }
    
    // TODO: Still need to update the process vector before returning.
    return m_processes;
}

std::string System::Kernel()
{ 
    return m_kernel;
}

float System::MemoryUtilization()
{
    // TODO: Return the system's memory utilization
    return 0.0f;
}

std::string System::OperatingSystem()
{
    return m_operatingSystem;
}

int System::RunningProcesses()
{
    // TODO: Return the number of processes actively running on the system
    return 0;
}

int System::TotalProcesses()
{
    // TODO: Return the total number of processes on the system
    return 0;
}

long int System::UpTime()
{
    // TODO: Return the number of seconds since the system started running
    // You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.
    return 0;
}
