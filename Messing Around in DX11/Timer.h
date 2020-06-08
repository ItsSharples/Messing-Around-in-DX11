#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <mutex>

// Alt Implementation, Might be better, might be worse, idrk.
#define TIME_PAIR

namespace
{
#ifdef TIME_PAIR
	std::vector<std::pair<std::chrono::system_clock::time_point, std::chrono::system_clock::time_point>> time_pairs;
#else
	std::vector<std::chrono::system_clock::time_point> time_points;
#endif // TIME_PAIR

	std::mutex timer_mutex;
}

namespace Time
{
	[[nodiscard]] const std::string prettyPrintTime();

	static void start() 
	// A FILO Time Queue
	{
		std::lock_guard<std::mutex> timelock(timer_mutex);
		auto time_point = std::chrono::system_clock::now();

#ifdef TIME_PAIR
		time_pairs.emplace_back(time_point, time_point);
#else
		time_points.emplace_back(time_point);
#endif // TIME_PAIR
	}

	static void stop() 
	{
		std::lock_guard<std::mutex> timelock(timer_mutex);

#ifdef TIME_PAIR
		time_pairs.back().second = std::chrono::system_clock::now();
#else
		time_points.back() = std::chrono::system_clock::now();
#endif // TIME_PAIR
	}


	[[nodiscard]] static std::chrono::duration<double> getElapsedSeconds()
	{
		std::lock_guard<std::mutex> timelock(timer_mutex);


#ifdef TIME_PAIR
		auto t_points = time_pairs.back();
		auto end = t_points.second;
		auto start = t_points.first;
#else
		// Too few stuff to deal with
		if(time_points.size() < 2)
		{
			return {};
		}

		// Get the last two time points
		auto t_points = time_points.rbegin();
		auto end = *t_points;
		t_points++;
		auto start = *t_points;
#endif // TIME_PAIR

		// If the pair has only started
		if(end == start)
		{
			// Stop the pair
			stop();
			// Try again
			return getElapsedSeconds();
		}

		std::chrono::duration<double> delta = end - start;

		// Pairs only need to pop the last pair, the normie vector needs to pop start/end
#ifdef TIME_PAIR
		time_pairs.pop_back();
#else
		time_points.pop_back();
		time_points.pop_back();
#endif // TIME_PAIR

		return delta;
	};


	[[nodiscard]] static std::chrono::duration<double> getElapsedMilliseconds()
	{
		return getElapsedSeconds() * 1000.f;
	}
};
