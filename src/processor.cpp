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
    UpdateCoreUtilization();
    return &m_cores;
}

void Processor::UpdateCoreUtilization()
{
    int currentCore {0};
    while (currentCore < m_coreCount)
    {
        std::string check {};
        std::string expectedCore {"cpu" + std::to_string(currentCore)};
        std::ifstream filestreamThen(m_statPath);
        if (filestreamThen.is_open())
        {
            filestreamThen.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            filestreamThen >> check;
            if (check == expectedCore)
            {
                filestreamThen >> m_cpuUserThen >> m_cpuNiceThen >> m_cpuSystemThen >> m_cpuIdleThen;
            }
            filestreamThen.close();
        }
        usleep(500000);
        std::ifstream filestreamNow(m_statPath);
        if (filestreamNow.is_open())
        {
            filestreamNow.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            filestreamNow >> check;
            if (check == expectedCore)
            {
                filestreamNow >> m_cpuUserNow >> m_cpuNiceNow >> m_cpuSystemNow >> m_cpuIdleNow;
            }
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
    std::string check {};
    int coreCount {0};

    std::ifstream filestream(m_statPath);
    if (filestream.is_open())
    {
        while (!filestream.eof())
        {
            std::string expectedCore {"cpu" + std::to_string(coreCount)};
            filestream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            filestream >> check;
            if (check == expectedCore)
            {
                ++coreCount;
            }
            else
            {
                break;
            }
        }
    }
    return coreCount;
}

float Processor::Utilization()
{
    std::string id {};

    std::ifstream filestreamThen(m_statPath);
    if (filestreamThen.is_open())
    {
        filestreamThen >> id >> m_cpuUserThen >> m_cpuNiceThen >> m_cpuSystemThen >> m_cpuIdleThen;
        filestreamThen.close();
    }
    usleep(500000);
    std::ifstream filestreamNow(m_statPath);
    if (filestreamNow.is_open())
    {
        filestreamNow >> id >> m_cpuUserNow >> m_cpuNiceNow >> m_cpuSystemNow >> m_cpuIdleNow;
    }

    m_sumThen = m_cpuUserThen + m_cpuNiceThen + m_cpuSystemThen + m_cpuIdleThen;
    m_sumNow = m_cpuUserNow + m_cpuNiceNow + m_cpuSystemNow + m_cpuIdleNow;

    if (m_sumNow - m_sumThen > 0) // Avoid divide by zero in case of stream error.
    {
        return (m_sumNow - m_sumThen) - (m_cpuIdleNow - m_cpuIdleThen) / (m_sumNow - m_sumThen);
    }
    return 0.0f;
}

