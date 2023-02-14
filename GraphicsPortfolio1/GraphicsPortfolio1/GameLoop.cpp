#include "GameLoop.h"

using namespace std::chrono;

void GameLoop::AddTimePoint(const string& key)
{
    m_time_point_table_.emplace(key, system_clock::now());
}

void GameLoop::EraseTimePoint(const string& key)
{
    if (m_time_point_table_.find(key) != m_time_point_table_.end())
    {
        m_time_point_table_.erase(key);
    }
}

void GameLoop::UpdateTimePoint(const string& key)
{
    if (m_time_point_table_.find(key) != m_time_point_table_.end())
    {
        m_time_point_table_[key] = system_clock::now();
    }
}


float GameLoop::GetElapsed(const string& key)
{
    float result = 0.f;
    if (m_time_point_table_.find(key) != m_time_point_table_.end())
    {
        duration<float> dt_duration = system_clock::now() - m_time_point_table_.at(key);
        result = dt_duration.count();
    }
    return result;
}
