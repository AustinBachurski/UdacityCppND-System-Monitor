#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

System::System():
m_cpu{},
m_kernel{},
m_operatingSystem{},
m_processes{}
{
    m_kernel = LinuxParser::Kernel();
    m_operatingSystem = LinuxParser::OperatingSystem();
    /*
    Kernel and Operating System information won't change while the
    tool is open - retrieve the information once and store it.
    */
}

Processor& System::Cpu()
{
    return m_cpu;
}

std::string System::Kernel()
{ 
    return m_kernel;
}

float System::MemoryUtilization()
{
    return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem()
{
    return m_operatingSystem;
}

std::vector<Process>& System::Processes()
{ 
    m_processes.clear();

    for (const int pid : LinuxParser::Pids())
    {
        m_processes.emplace_back(Process(
            LinuxParser::UpTime(pid),
            LinuxParser::Command(pid),
            LinuxParser::CpuUtilization(pid),
            LinuxParser::Ram(pid),
            pid,
            LinuxParser::User(pid)
            ));
    }
    
    std::sort(m_processes.begin(), m_processes.end());
    return m_processes;
}

int System::RunningProcesses()
{
    return LinuxParser::RunningProcesses();
}

int System::TotalProcesses()
{
    return LinuxParser::TotalProcesses();
}

long int System::UpTime()
{
    return LinuxParser::UpTime();
}