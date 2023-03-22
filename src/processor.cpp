#include "processor.h"

Processor::Processor() :
m_coreCount{},
m_cores{},
m_cpuIdleNow{},
m_cpuIdleThen{},
m_cpuNiceNow{},
m_cpuNiceThen{},
m_cpuSystemNow{},
m_cpuSystemThen{},
m_cpuUserNow{},
m_cpuUserThen{},
m_sleepTime{},
m_statPath{ "/proc/stat" },
m_sumNow{},
m_sumThen{}
{
    m_coreCount = SetCoreCount();
    m_cores.resize(GetCoreCount());
    m_sleepTime = 1 / (sysconf(_SC_CLK_TCK) / 1000.0) * 2000; // Two clock cycles in microseconds.
}

std::vector<float>& Processor::Cores()
{
    UpdateCoreUtilization();
    return m_cores;
}

int Processor::GetCoreCount()
{
    return m_coreCount;
}

void Processor::UpdateCoreUtilization()
{
    int currentCore {0};
    while (currentCore < m_coreCount)
    {
        bool searching { true };
        std::string checkValue {};
        std::string expectedCore {"cpu" + std::to_string(currentCore)};
        std::ifstream filestreamThen(m_statPath);
        if (filestreamThen.is_open())
        {
            while (searching)
            {
                filestreamThen.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                filestreamThen >> checkValue;

                if (checkValue == expectedCore)
                {
                    searching = false;
                    filestreamThen >> m_cpuUserThen >> m_cpuNiceThen >> m_cpuSystemThen >> m_cpuIdleThen;
                    filestreamThen.close();
                }
                
            }
        }
        usleep(m_sleepTime);
        std::ifstream filestreamNow(m_statPath);
        if (filestreamNow.is_open())
        {
            searching = true;
            while (searching)
            {
                filestreamNow.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                filestreamNow >> checkValue;
                if (checkValue == expectedCore)
                {
                    searching = false;
                    filestreamNow >> m_cpuUserNow >> m_cpuNiceNow >> m_cpuSystemNow >> m_cpuIdleNow;
                    filestreamNow.close();
                }
            } 
        }

        m_sumThen = m_cpuUserThen + m_cpuNiceThen + m_cpuSystemThen + m_cpuIdleThen;
        m_sumNow = m_cpuUserNow + m_cpuNiceNow + m_cpuSystemNow + m_cpuIdleNow;

        if (m_sumNow - m_sumThen) // Avoid divide by zero.
        {
            m_cores[currentCore] = ((m_sumNow - m_sumThen) - (m_cpuIdleNow - m_cpuIdleThen)) / (m_sumNow - m_sumThen);
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
    std::ifstream filestreamThen(m_statPath);
    if (filestreamThen.is_open())
    {
        filestreamThen.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        filestreamThen  >> m_cpuUserThen >> m_cpuNiceThen >> m_cpuSystemThen >> m_cpuIdleThen;
        filestreamThen.close();
    }

    usleep(m_sleepTime);

    std::ifstream filestreamNow(m_statPath);
    if (filestreamNow.is_open())
    {
        filestreamNow.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
        filestreamNow  >> m_cpuUserNow >> m_cpuNiceNow >> m_cpuSystemNow >> m_cpuIdleNow;
    }

    m_sumThen = m_cpuUserThen + m_cpuNiceThen + m_cpuSystemThen + m_cpuIdleThen;
    m_sumNow = m_cpuUserNow + m_cpuNiceNow + m_cpuSystemNow + m_cpuIdleNow;
    if (m_sumNow - m_sumThen) // Avoid divide by zero.
    {
        return ((m_sumNow - m_sumThen) - (m_cpuIdleNow - m_cpuIdleThen)) / (m_sumNow - m_sumThen);
    }
    return 0.0f;
}