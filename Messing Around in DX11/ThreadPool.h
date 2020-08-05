// Based on https://github.com/vit-vit/CTPL
#pragma once
/*
#include <mutex>
#include <functional>
#include <future>
#include <queue>

#include <cstdarg>// Magic Anynumber of Args Module UwU

namespace
{
std::mutex shared_mutex;


template <typename T>
std::function<void*()> castToVoidFunc (T* (*func)())
{
  return [=](){ return func(); };
}



}

template<typename ReturnType>
struct Job
{
	//typedef std::function<ReturnType> BoundFunction;
public:
	template<class BoundFunction>
	Job(BoundFunction func)
	{
		if(std::is_bind_expression<decltype(func)>)
		{
			function = &func;
		}
		else
		{
			throw std::exception("Not a Bound Function");
		}
	};
	~Job()	= default;

	template<typename... Args>
	void run(Args... args) 
	{
		std::lock_guard<std::mutex> Lock(shared_mutex); 
		job_promise.set_value(function(args...)); // Do the job and keep the returned value
	};

	auto resolve() -> ReturnType { return job_future.get(); };



protected:
private:
	void* function; 

	std::promise<ReturnType> job_promise;
	std::future<ReturnType> job_future = job_promise.get_future();
};

namespace JobFactory
{
template<typename Function, typename... Args>
static auto make_job(Function function, Args... args) -> Job<decltype(function)>*
{
	auto bound_function = std::bind(function, args...);
	return new Job<decltype(function)>(bound_function);
}
}


class ThreadPool
{
public:
	ThreadPool() = default;
	~ThreadPool() = default;

	size_t getPoolSize() = delete;
	size_t getFreeThreadCount() = delete;

	template<typename ReturnType>
	bool addJob(Job<ReturnType>* job) = delete;

	void runAll()
	{
		auto lambda = [&](int i){return i; };
		auto bound_lambda = std::bind(lambda, std::placeholders::_1);
		auto* job = new Job<int>(bound_lambda);
		
		auto RAII_Job = JobFactory::make_job(lambda, 5);
		
		int b = bound_lambda(5);
		job->run();

		auto result = job->resolve();

	}

protected:
private:

	std::atomic<bool>	m_is_full;
	std::atomic<size_t> m_pool_size;
	std::atomic<size_t> m_free_threads;

	std::queue<std::thread> thread_queue;
};

*/