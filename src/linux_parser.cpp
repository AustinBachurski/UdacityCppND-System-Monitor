#include <dirent.h>
#include <filesystem>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"

long LinuxParser::ActiveJiffies()
{
  // TODO: Read and return the number of active jiffies for the system
  return 0;
}

long LinuxParser::ActiveJiffies(int pid[[maybe_unused]])
{
  // TODO: Read and return the number of active jiffies for a PID
  // REMOVE: [[maybe_unused]] once you define the functiona
  return 0;
}

std::string LinuxParser::Command(int pid[[maybe_unused]])
{
  // TODO: Read and return the command associated with a process
  // REMOVE: [[maybe_unused]] once you define the function
  return "";
}

int LinuxParser::CoreCount()
{
    // Return cpu core count by parsing /proc/stat
    return 0;
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

long LinuxParser::IdleJiffies()
{
  // TODO: Read and return the number of idle jiffies for the system
    return 0;
}

bool LinuxParser::IsNumber(const std::string& value)
{
    return std::all_of(value.begin(), value.end(), isdigit);
}

long LinuxParser::Jiffies()
{
  // TODO: Read and return the number of jiffies for the system
    return 0;
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

std::string LinuxParser::Ram(int pid[[maybe_unused]])
{
  // TODO: Read and return the memory used by a process
  // REMOVE: [[maybe_unused]] once you define the function
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

std::string LinuxParser::Uid(int pid[[maybe_unused]])
{
  // TODO: Read and return the user ID associated with a process
  // REMOVE: [[maybe_unused]] once you define the function
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

long LinuxParser::UpTime(int pid[[maybe_unused]])
{
  // TODO: Read and return the uptime of a process
  // REMOVE: [[maybe_unused]] once you define the function
  return 0;
}

std::string LinuxParser::User(int pid[[maybe_unused]])
{
  // TODO: Read and return the user associated with a process
  // REMOVE: [[maybe_unused]] once you define the function
  return "";
}