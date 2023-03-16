#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include "process.h"
#include "processor.h"

class System
{
  public:
    System();
    Processor& Cpu();
    std::vector<Process>& Processes(); 
    float MemoryUtilization();          // TODO: See src/system.cpp
    long UpTime();                      // TODO: See src/system.cpp
    int TotalProcesses();               // TODO: See src/system.cpp
    int RunningProcesses();             // TODO: See src/system.cpp
    std::string Kernel();
    std::string OperatingSystem();

  private:
    Processor m_cpu;
    std::vector<Process> m_processes;
    std::string m_kernel;
    std::string m_operatingSystem;

};

#endif