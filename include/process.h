#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process
{
 private:
    long int m_age;
    std::string m_command;
    std::string m_cpuUtilization;
    std::string m_memory;
    long m_pid;
    std::string m_user;

 public:
    Process(
        long int age,
        std::string command,
        std::string cpuUtilization,
        std::string memory, 
        int pid, 
        std::string user
    );
    int Pid();
    std::string User();
    std::string Command();
    std::string CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process const& a) const; 
};

#endif