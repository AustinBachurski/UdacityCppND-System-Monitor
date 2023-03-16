#include "processor.h"

Processor::Processor() :
m_coreCount{ SetCoreCount() },
m_cores{}, // Pre-allocate size from core count?
m_cpuUserThen{},
m_cpuNiceThen{},
m_cpuSystemThen{},
m_cpuIdleThen{},
m_cpuUserNow{},
m_cpuNiceNow{},
m_cpuSystemNow{},
m_cpuIdleNow{},
m_sumThen{},
m_sumNow{}
{}

std::vector<float>* Processor::Cores()
{
    return &m_cores;
}

void Processor::CoreUtilization()
{
    int currentCore {0};
    while (currentCore < m_coreCount)
    {
        std::string previousCores {"cpu" + std::to_string(currentCore)};
        std::ifstream filestreamNow(m_statPath);
        if (filestreamNow.is_open())
        {
            filestreamNow.ignore(std::numeric_limits<std::streamsize>::max(), previousCores);
            filestreamNow >> m_cpuUserNow >> m_cpuNiceNow >> m_cpuSystemNow;
        }
        Sleep(500);
        std::ifstream filestreamThen(m_statPath);
        if (filestreamThen.is_open())
        {
            filestreamThen.ignore(std::numeric_limits<std::streamsize>::max(), previousCores);
            filestreamThen>> m_cpuUserThen >> m_cpuNiceThen >> m_cpuSystemThen;
        }

        m_sumThen = m_cpuUserThen + m_cpuNiceThen + m_cpuSystemThen + m_cpuIdleThen;
        m_sumNow = m_cpuUserNow + m_cpuNiceNow + m_cpuSystemNow + m_cpuIdleNow;

        if (m_sumNow - m_sumThen > 0) // Avoid divide by zero in case of stream error.
        {
            m_cores[currentCore] = (m_sumNow - m_sumThen) - (m_cpuIdleNow - m_cpuIdleThen) / (m_sumNow - m_sumThen);
            ++currentCore;
        }
    }
}

bool Processor::Multicore()
{
    return m_coreCount > 1;
}

int Processor::SetCoreCount()
{
    int count {0};

    std::ifstream filestream(m_statPath);
    if (filestream.is_open())
    {
        while (!filestream.eof())
        {
            filestream.ignore(std::numeric_limits<std::streamsize>::max(), "cpu" + count);
            if (!filestream.eof())
            {
                ++count;
            }
        }
    }
    return count;
}

float Processor::Utilization()
{
    std::ifstream filestreamThen(m_statPath);
    if (filestreamThen.is_open())
    {
        filestreamThen >> m_cpuUserThen >> m_cpuNiceThen >> m_cpuSystemThen;
    }
    Sleep(500);
    std::ifstream filestreamNow(m_statPath);
    if (filestreamNow.is_open())
    {
        filestreamNow >> m_cpuUserNow >> m_cpuNiceNow >> m_cpuSystemNow;
    }

    m_sumThen = m_cpuUserThen + m_cpuNiceThen + m_cpuSystemThen + m_cpuIdleThen;
    m_sumNow = m_cpuUserNow + m_cpuNiceNow + m_cpuSystemNow + m_cpuIdleNow;

    if (m_sumNow - m_sumThen > 0) // Avoid divide by zero in case of stream error.
    {
        return (m_sumNow - m_sumThen) - (m_cpuIdleNow - m_cpuIdleThen) / (m_sumNow - m_sumThen);
    }
    return 0.0f;
}

