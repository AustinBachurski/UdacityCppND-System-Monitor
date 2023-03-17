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
m_processes{},
m_kernel{},
m_operatingSystem{}
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

std::vector<Process>& System::Processes()
{ 
    for (const int pid : LinuxParser::Pids())
    {
        m_processes.emplace_back(Process(
            pid,
            LinuxParser::CpuUtilization(pid),
            LinuxParser::Command(pid),
            LinuxParser::Ram(pid),
            LinuxParser::User(pid),
            LinuxParser::UpTime(pid)
            ));
    }
    std::sort(m_processes.begin(), m_processes.end(), Process::operator<())
    return &m_processes;
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