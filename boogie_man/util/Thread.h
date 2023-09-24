#pragma once
#include "Log.h"
#include <functional>
#include <queue>
#include <thread>
#include <mutex>

class ThreadPool
{
public:

	ThreadPool(size_t numberOfThreads)
	{
		for (size_t i = 0; i < numberOfThreads; i++)
		{
			threads.emplace_back([this] {
				while (true)
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(mutex);
						condition.wait(lock, [this] {return !tasks.empty() || stop; });
						if (stop && tasks.empty())
						{
							return;
						}
						task = std::move(tasks.front());
						tasks.pop();
					}
					task();
					LOG_INF("Thread executed the task!");
				}
			});
		}

		LOG_INF("Thread pool initialized!");
	}

	template<class F>
	void enqueue(F&& task)
	{
		{
			std::unique_lock<std::mutex> lock(mutex);
			tasks.emplace(std::forward<F>(task));
		}
		condition.notify_one();
	}

	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(mutex);
			stop = true;
		}

		condition.notify_all();
		for (auto &thread : threads)
		{
			thread.join();
			LOG_INF("Thread is distrupted!");
		}
	}


private:

	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	std::mutex mutex;
	std::condition_variable condition;
	bool stop = false;
};
