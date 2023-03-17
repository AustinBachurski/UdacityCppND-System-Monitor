#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <fstream>
#include <limits>
#include <sstream>
#include <unistd.h>
#include <vector>

class Processor
{
  public:
    Processor();
    std::vector<float>* Cores();
    bool Multicore();
    int SetCoreCount();
    float Utilization();

  private:
    void UpdateCoreUtilization();
    const int m_coreCount;
    std::vector<float> m_cores;
    float m_cpuUserThen;
    float m_cpuNiceThen;
    float m_cpuSystemThen;
    float m_cpuIdleThen;
    float m_cpuUserNow;
    float m_cpuNiceNow;
    float m_cpuSystemNow;
    float m_cpuIdleNow;
    const std::string m_statPath {"/proc/stat"};
    float m_sumThen;
    float m_sumNow;
};

#endif