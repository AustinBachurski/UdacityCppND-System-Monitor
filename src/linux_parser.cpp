#include <dirent.h>
#include <filesystem>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

std::string LinuxParser::Command(int pid)
{
    std::string command {};

    std::ifstream filestream("/proc/" + std::to_string(pid) + "/cmdline");
    if (filestream.is_open())
    {
        filestream >> command;
        return command;
    }
  return "";
}

float LinuxParser::CpuUtilization(int pid)
{ // Can't say I'm a fan of how messy this function is, but if it works...
    const int userTimePosition { 14 };
    const int childKernelTimePosition { 17 };
    const int startTimePosition { 22 };

    float upTimeThen = Uptime(); // Already in seconds!
    float userTimeThen {};
    float kernelTimeThen {};
    float childUserTimeThen {};
    float childKernelTimeThen {};
    float timeRunningThen {};
    float timeActiveThen {};
    float startTime {};

    float upTimeNow = Uptime(); // Already in seconds!
    float userTimeNow {};
    float kernelTimeNow {};
    float childUserTimeNow {};
    float childKernelTimeNow {};
    float timeRunningNow {};
    float timeActiveNow {};

    std::ifstream filestreamThen("/proc/" + std::to_string(pid) + "/stat");
    if (filestreamThen.is_open())
    {
        for (int streamPosition = 0; streamPosition < userTimePosition; ++streamPosition)
        {
            filestreamThen >> userTimeThen;
        }
        filestreamThen >> kernelTimeThen >> childUserTimeThen >> childKernelTimeThen;
        for (int streamPosition = childKernelTimePosition; streamPosition < startTimePosition; ++streamPosition)
        {
            filestreamThen >> startTime;
        }
        filestreamThen.close();
        startTime /= sysconf(_SC_CLK_TCK);
        float timeRunningThen = upTimeThen - startTime;
        float timeActiveThen = (
            userTimeThen + kernelTimeThen + childUserTimeThen + childKernelTimeThen) / sysconf(_SC_CLK_TCK);
    }
    usleep(500000);
    std::ifstream filestreamNow("/proc/" + std::to_string(pid) + "/stat");
    if (filestreamNow.is_open())
    {
        for (int streamPosition = 0; streamPosition < userTimePosition; ++streamPosition)
        {
            filestreamNow >> userTimeNow;
        }
        filestreamNow >> kernelTimeNow >> childUserTimeNow >> childKernelTimeNow;
        float timeRunningNow = upTimeNow - startTime;
        float timeActiveNow = (
            userTimeNow + kernelTimeNow + childUserTimeNow + childKernelTimeNow) / sysconf(_SC_CLK_TCK);
        
        return (timeActiveNow - timeActiveThen) / (timeRunningNow - timeRunningThen);
    }
    return 0.0f;
}

int LinuxParser::GetProcesses(const std::string state)
{
    std::string check {};
    std::string value {};

    std::ifstream filestream("/proc/stat");
    if (filestream.is_open())
    {
        while (!filestream.eof())
        {
            filestream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            filestream >> check;
            if (check == state)
            {
                filestream >> value;
                if (IsNumber(value))
                {
                    return std::stoi(value);
                }  
            }
        }  
    }
    return 0;
}

bool LinuxParser::IsNumber(const std::string& value)
{
    /*
    Not sure if this is good practice or not - it felt like a
    don't repeat yourself situation with all the arguments, 
    but I'm just calling one function from another 
    function - should I not be doing this? - Thank you.
    */
    return std::all_of(value.begin(), value.end(), isdigit);
}

std::string LinuxParser::Kernel()
{
    std::string kernel {};
    std::string version {};
    std::string os {};
    std::string line {};

    std::ifstream stream("/proc/version");
    if (stream.is_open())
    {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
    }
    return kernel;
}

float LinuxParser::MemoryUtilization()
{
    float free {};
    std::string line {};
    std::string key {};
    std::string suffix {};
    float total {};
    std::string value {};

    std::ifstream stream("/proc/meminfo");
    if (stream.is_open())
    {
        while(std::getline(stream, line))
        {
            std::istringstream linestream(line);
            linestream >> key >> value >> suffix;

            if (key == "MemTotal:")
            {
                if (IsNumber(value))
                {
                    total = std::stof(value);
                }
            }
            else if (key == "MemFree:")
            {
                if (IsNumber(value))
                {
                    free = std::stof(value);
                    break;
                }
            }
        }
    }
    return (total - free) / total;
}

std::string LinuxParser::OperatingSystem()
{
    std::string line {};
    std::string key {};
    std::string value {};

    std::ifstream filestream("/etc/os-release");
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);

            while (linestream >> key >> value)
            {
                if (key == "PRETTY_NAME")
                {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

std::vector<int> LinuxParser::Pids()
{
    std::vector<int> m_pids {};

    for (const auto& file : std::filesystem::directory_iterator("/proc"))
    {
        if (file.is_directory())
        {
            std::string filename(file.path().filename());
            if (IsNumber(filename))
            {
                m_pids.push_back(std::stoi(filename));
            }
        }
    }
    return m_pids;
}

std::string LinuxParser::Ram(int pid)
{
    std::string check {};
    std::string usage {};

    std::ifstream filestream("/proc/" + std::to_string(pid) + "/status");
    if (filestream.is_open())
    {
        filestream.ignore(std::numeric_limits<std::streamsize>max(), '\n');
        filestream >> check;
        if (check == "VMSize:")
        {
            filestream >> usage;
            return usage;
        }
    }
    return "";
}

int LinuxParser::RunningProcesses()
{
    return GetProcesses(m_runningProcesses);
}

int LinuxParser::TotalProcesses()
{
    return GetProcesses(m_totalProcesses);
}

std::string LinuxParser::Uid(int pid)
{
    std::string check {};
    std::string uid {};

    std::ifstream filestream("/proc/" + std::to_string(pid) + "/status");
    if (filestream.open())
    {
        while (!filestream.eof())
        {
            filestream.ignore(std::numeric_limits<std::streamsize>max(), '\n');
            filestream >> check;
            if (check == "Uid:")
            {
                filestream >> uid;
                return uid;
            }
        }
    }
    return "";
}

long LinuxParser::UpTime()
{
    std::string line {};

    std::ifstream filestream ("/proc/uptime");
    if (filestream.is_open())
    {
        filestream >> line;
        if (IsNumber(line))
        {
            return std::stol(line);
        }
    }
    return 0;
}

long LinuxParser::UpTime(int pid)
{
    const int desiredPosition { 22 };
    std::string upTime {};

    std::ifstream filestream("/proc/" + std::to_string(pid) + "/stat");
    if (filestream.is_open())
    {
        for (int streamPosition = 0; streamPosition < desiredPosition; ++streamPosition)
        {
            filestream >> upTime;
        }
        if (IsNumber(upTime))
        {
            return std::stol(upTime) / sysconf(_SC_CLK_TCK);
        }
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

    std::ifstream filestream("/etc/passwd");
    if (filestream.open())
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ':', ' ');
            line >> user >> password >> check;
            if (check == uid)
            {
                return user;
            }
        }
    }
  return "";
}