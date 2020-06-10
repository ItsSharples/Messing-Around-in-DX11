// Based on https://github.com/vit-vit/CTPL
#pragma once

#include <mutex>
#include <functional>
#include <future>
#include <queue>

namespace
{
std::mutex shared_mutex;
}

template<class Function>
struct Job
{
public:
	typedef Function Functor();

	Job(Functor* func) { this->function = func; };
	~Job()	= default;

	template<typename... Args>
	void run() 
	{
		std::lock_guard<std::mutex> Lock(shared_mutex); 
		job_promise.set_value(Function(Args...));
	};

	decltype(Function()) resolve() { return job_future.get(); };

protected:
private:
	Functor* function; 

	std::promise<decltype(Function())> job_promise;
	std::future<decltype(Function())> job_future = job_promise.get_future();
};

class ThreadPool
{
public:
	ThreadPool() = default;
	~ThreadPool() = default;

	size_t getPoolSize() = delete;
	size_t getFreeThreadCount() = delete;

	template<typename Function>
	bool addJob(Job<Function>* job) = delete;

	void runAll()
	{
		auto lambda = [](){return 1; };
		auto* job = new Job<int>(lambda);

		job->run();

		int result = job->resolve();

	}

protected:
private:

	std::atomic<bool>	m_is_full;
	std::atomic<size_t> m_pool_size;
	std::atomic<size_t> m_free_threads;

	std::queue<std::thread> thread_queue;
};

