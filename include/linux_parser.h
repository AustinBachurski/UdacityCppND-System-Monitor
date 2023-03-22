#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser
{
    const std::string m_command { "/cmdline" };
    const std::string m_meminfo { "/proc/meminfo" };
    const std::string m_modeCpu { "cpu" };
    const std::string m_modeRam { "ram" };
    const std::string m_os { "/etc/os-release" };
    const std::string m_password { "/etc/passwd" };
    const std::string m_proc { "/proc/" };
    const std::string m_procStat { "/proc/stat" };
    const int m_startOfStream { 0 };
    const int m_startTimePosition { 22 }; // From man pages.
    const std::string m_stat { "/stat" };
    const std::string m_status { "/status" };
    const std::string m_uptime { "/proc/uptime" };
    const int m_userTimePosition { 14 }; // From man pages.
    const std::string m_version { "/proc/version" };

    std::string Command(const int pid);
    std::string CpuUtilization(const int pid);
    std::string GetValueFromKeyAsString(std::ifstream& stream, const std::string searchString);
    bool IsStringNumber(const std::string& string);
    std::string Kernel();
    float MemoryUtilization();
    std::string OperatingSystem();
    std::vector<int> Pids();
    std::string Ram(const int pid);
    int RunningProcesses();
    std::string SetFloatPrecisionAsString(const float value, const int decimalPlaces, const std::string& mode);
    int TotalProcesses();
    std::string Uid(const int pid);
    long UpTime();
    float UpTime(const int pid);
    std::string User(const int pid);
};

#endif