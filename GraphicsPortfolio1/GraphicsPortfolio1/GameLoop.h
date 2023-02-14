#pragma once
#include <map>
#include <chrono>
#include <string>

using std::string;
using std::map;
using std::pair;
using std::chrono::system_clock;

class GameLoop
{

protected:
	map<string, system_clock::time_point> m_time_point_table_;

public:
	void AddTimePoint(const string& key);
	void EraseTimePoint(const string& key);
	void UpdateTimePoint(const string& key);

public:
	float GetElapsed(const string& key);
};

