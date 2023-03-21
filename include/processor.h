#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <fstream>
#include <limits>
#include <sstream>
#include <unistd.h>
#include <vector>

class Processor
{
 private:
    void UpdateCoreUtilization();
    int m_coreCount;
    std::vector<float> m_cores;
    float m_cpuIdleNow;
    float m_cpuIdleThen;
    float m_cpuNiceNow;
    float m_cpuNiceThen;
    float m_cpuSystemNow;
    float m_cpuSystemThen;
    float m_cpuUserNow;
    float m_cpuUserThen;
    long m_sleepTime;
    const std::string m_statPath;
    float m_sumNow;
    float m_sumThen;

 public:
    Processor();
    std::vector<float>& Cores();
    int GetCoreCount();
    bool Multicore();
    int SetCoreCount();
    float Utilization();
};

#endif