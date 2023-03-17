#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser
{
    // Delete this before submitting!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    const std::string kProcDirectory{"/proc/"};
    const std::string kCmdlineFilename{"/cmdline"};
    const std::string kCpuinfoFilename{"/cpuinfo"};
    const std::string kStatusFilename{"/status"};
    const std::string kStatFilename{"/stat"};
    const std::string kUptimeFilename{"/uptime"};
    const std::string kMeminfoFilename{"/meminfo"};
    const std::string kVersionFilename{"/version"};
    const std::string kOSPath{"/etc/os-release"};
    const std::string kPasswordPath{"/etc/passwd"};



    const std::string m_runningProcesses {"procs_running"};
    const std::string m_totalProcesses {"processes"};

    enum CPUStates // Pointless, remove!
    {
        kUser_ = 0,
        kNice_,
        kSystem_,
        kIdle_,
        kIOwait_,
        kIRQ_,
        kSoftIRQ_,
        kSteal_,
        kGuest_,
        kGuestNice_
    };

    std::string Command(int pid);
    float CpuUtilization(int pid);
    int GetProcesses(const std::string state);
    long IdleJiffies();
    bool IsNumber(const std::string& value);
    long Jiffies();
    std::string Kernel();
    float MemoryUtilization();
    std::string OperatingSystem();
    std::vector<int> Pids();
    std::string Ram(int pid);
    int RunningProcesses();
    int TotalProcesses();
    std::string Uid(int pid);
    long UpTime();
    long int UpTime(int pid);
    std::string User(int pid);
};

#endif