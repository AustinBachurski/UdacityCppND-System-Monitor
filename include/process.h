#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process
{
 public:
    Process(
        int pid, 
        float utilization, 
        std::string command, 
        std::string memory, 
        std::string user, 
        long int age
        );

    int Pid();
    std::string User();
    std::string Command();
    float CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
    int m_pid;
    float m_cpuUtilization;
    std::string m_generatedCommand;
    std::string m_memoryUtilization;
    std::string m_user;
    long int m_age;

};

#endif