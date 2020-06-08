#include "pch.h"
#include "Timer.h"

#include "SimpleHelper.h"

#define TIMER_HAS_DATE 1

namespace
{
std::string pad_string(std::string str, size_t desired, char pad_val = '0')
{
	while (desired > str.length())
    {
		str = pad_val + str;
    }
    return str;
}
}
// TODO Sort this out
/*
inline std::string timestamp() 
{
//get the time
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(tp); 
	std::tm gmt{};
	//gmtime_r(&tt, &gmt);
	std::chrono::duration<double> fractional_seconds = (tp - std::chrono::system_clock::from_time_t(tt)) + std::chrono::seconds(gmt.tm_sec);
	//format the string
	std::string buffer("year/mo/dy hr:mn:sc.xxxxxx");
	sprintf(&buffer.front(), "%04d/%02d/%02d %02d:%02d:%09.6f",	gmt.tm_year + 1900, gmt.tm_mon + 1, gmt.tm_mday, gmt.tm_hour, gmt.tm_min, fractional_seconds.count()); 
	return buffer;
}
*/
const std::string Time::prettyPrintTime()
{
	time_t raw_time;
	time(&raw_time); // Populate with the time
	auto local_time = asctime(localtime(&raw_time)); // Convert to localtime
	auto now_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
	auto just_ms = now_in_ms % 1000; // 1 Thousand milliseconds is 1 seconds, so mod them all away

#if TIMER_HAS_DATE
	std::string pretty_time{ "year-mn-dy hr:mn:ss:" }; // Convert to String
	strftime(&pretty_time.front(), 128, "%F %T:", localtime(&raw_time));
#else
	std::string pretty_time{ "hr:mn:ss:" }; // Convert to String
	strftime(&pretty_time.front(), 128, "%T:", localtime(&raw_time));
#endif
	pretty_time += pad_string(std::to_string(just_ms.count()), 3);
	return pretty_time;
}
