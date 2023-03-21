#include <dirent.h>
#include <filesystem>
#include <limits>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

std::string LinuxParser::Command(int pid)
{
    std::string command {};

    std::ifstream filestream(m_proc + std::to_string(pid) + m_command);
    if (filestream.is_open())
    {
        filestream >> command;
        return command;
    }
  return "";
}

float LinuxParser::CpuUtilization(int pid)
{
    float userTime {};
    float kernelTime {};
    float childUserTime {};
    float childKernelTime {};
    float timeActive {};
    float timeRunning { UpTime(pid) };

    std::ifstream filestreamThen(m_proc + std::to_string(pid) + m_stat);
    if (filestreamThen.is_open())
    {
        for (int streamPosition = m_startOfStream; streamPosition < m_userTimePosition - 1; ++streamPosition)
        {
            filestreamThen.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        }

        filestreamThen >> userTime >> kernelTime >> childUserTime >> childKernelTime;
        timeActive = (userTime + kernelTime + childUserTime + childKernelTime) / sysconf(_SC_CLK_TCK);
        
        if (timeRunning)
        {
            return timeActive / timeRunning;
        }
    }
    return 0.0f;
}

std::string LinuxParser::Kernel()
{
    std::string kernel {};
    std::string version {};
    std::string os {};
    std::string line {};

    std::ifstream stream(m_version);
    if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

std::string LinuxParser::GetValueFromKeyAsString(std::ifstream& stream, const std::string searchString)
{
     std::string checkString {};
     std::string value {};

    if (stream.is_open())
    {
        if (stream.tellg())
        {
            stream.clear();
            stream.seekg(0);
        }
            
        while (!stream.eof())
        {
            stream >> checkString;
            if (checkString == searchString)
            {
                stream >> value;
                return value;
            }
            else
            {
                stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
    return "0";
}

bool LinuxParser::IsStringNumber(std::string& string)
{
    return std::all_of(string.begin(), string.end(), isdigit);
}

float LinuxParser::MemoryUtilization()
{
    std::ifstream filestream(m_meminfo);
    std::string freeString = GetValueFromKeyAsString(filestream, "MemFree:");
    std::string totalString = GetValueFromKeyAsString(filestream, "MemTotal:");

    if (IsStringNumber(freeString) && IsStringNumber(totalString))
    {
        float free = std::stof(freeString);
        float total = std::stof(totalString);
        if (total)
        {
            return (total - free) / total;
        }
    }
    return 0.0f;
}

std::string LinuxParser::OperatingSystem()
{
    std::string os {};
    
    std::ifstream filestream(m_os);
    if (filestream.is_open())
    {
        while (!filestream.eof())
        {
            filestream >> os;
            if (os.find("PRETTY") != std::string::npos)
            {
                os.erase(0, os.find("\""));
                os.erase(test.rfind("\""));
                return os;
            }
        }
    }
    return "";
}

std::vector<int> LinuxParser::Pids()
{
    std::vector<int> m_pids {};

    for (const auto& file : std::filesystem::directory_iterator(m_proc))
    {
        if (file.is_directory())
        {
            std::string pidString(file.path().filename());
            if (IsStringNumber(pidString))
            {
                m_pids.emplace_back(std::stoi(pidString));
            }
        }
    }
    return m_pids;
}

std::string LinuxParser::Ram(int pid) // Instructions state to use VmSize - see comment below.
{
    std::ifstream filestream(m_proc + std::to_string(pid) + m_status);
    std::string valueString = GetValueFromKeyAsString(filestream, "VmRSS:");
    if (IsStringNumber(valueString))
    {
        float value = std::stof(valueString);
        value /= 1000;
        return SetFloatPrecisionAsString(value, 2);
    }  
    return "0.0";
}
/*
Followed instructions initially but I was displaying more ram usage than my
system is equipped with.  After looking into why, I felt that VmRSS was more appropriate.

Source:
https://www.hackerearth.com/practice/notes/vivekprakash/technical-diving-into-memory-used-by-a-program-in-online-judges/

"VmSize is how much virtual memory the process has in total. 
    This includes all types of memory, both in RAM and swapped out. 
    These numbers can get skewed because they also include shared libraries."

"VmRSS in /proc/[pid]/statm is a useful data. 
    It shows how much memory in RAM is occupied by the process. 
    The rest extra memory has either been not used or has been swapped out."
*/

int LinuxParser::RunningProcesses()
{
    std::ifstream filestream(m_procStat);
    std::string countString = GetValueFromKeyAsString(filestream, "procs_running");
    if (IsStringNumber(countString))
    {
        return std::stoi(countString);
    }
    return 0;
}

std::string LinuxParser::SetFloatPrecisionAsString(float value, int decimalPlaces)
{
    std::string valueAsString = std::to_string(value);
    if (valueAsString.find('.') == 3)
    {
        return "100";
    }
    valueAsString.erase(valueAsString.find('.') + decimalPlaces);
    return valueAsString;
}

int LinuxParser::TotalProcesses()
{
    std::ifstream filestream(m_procStat);
    std::string countString = GetValueFromKeyAsString(filestream, "processes");
    if (IsStringNumber(countString))
    {
        return std::stoi(countString);
    }
    return 0;
}

std::string LinuxParser::Uid(int pid)
{
    std::ifstream filestream(m_proc + std::to_string(pid) + m_status);
    return GetValueFromKeyAsString(filestream, "Uid:");
}

long LinuxParser::UpTime()
{
    long time {};

    std::ifstream filestream (m_uptime);
    if (filestream.is_open())
    {
        filestream >> time;
        return time;
    }
    return 0;
}

float LinuxParser::UpTime(int pid)
{
    float startTime {};

    std::ifstream filestream(m_proc + std::to_string(pid) + m_stat);
    if (filestream.is_open())
    {
        for (int streamPosition = m_startOfStream; streamPosition < m_startTimePosition - 1; ++streamPosition)
        {
            filestream.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        }
        filestream >> startTime;
        return UpTime() - startTime / sysconf(_SC_CLK_TCK);

    }
    return 0;
}

std::string LinuxParser::User(int pid)
{
    std::string uid = Uid(pid);
    std::string user {};
    std::string line {};
    std::string password {};
    std::string check {};

    std::ifstream filestream(m_password);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> user >> password >> check;
            if (check == uid)
            {
                return user;
            }
        }
    }
  return "";
}