#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include "process.h"
#include "processor.h"

class System
{
 private:
    Processor m_cpu;
    std::string m_kernel;
    std::string m_operatingSystem;
    std::vector<Process> m_processes;

  public:
    System();
    Processor& Cpu();
    std::string Kernel();
    float MemoryUtilization();
    std::string OperatingSystem();
    std::vector<Process>& Processes();
    int RunningProcesses();
    int TotalProcesses();
    long UpTime();
};

#endif