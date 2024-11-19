#pragma once

#include "stdafx.h"
#include "api/utils/ThreadQueue.h"

namespace Chimp {
	// Create a thread pool that can execute tasks asynchronously
	// wip tasks will be discarded upon destruction of the thread pool
	class ThreadPool {
	public:
		typedef std::function<void()> Task;
		struct TaskDescription {
			Task TaskFunction; // This is the actual task
			std::unique_ptr<std::lock_guard<std::mutex>> Lock; // Lock a mutex while the task is running (may be nullptr)
			TaskDescription(const Task& task, std::unique_ptr<std::lock_guard<std::mutex>> lock) :
				TaskFunction(task),
				Lock(std::move(lock))
			{
			}
			TaskDescription(TaskDescription&& other) noexcept {
				TaskFunction = std::move(other.TaskFunction);
				Lock = std::move(other.Lock);
			}
			DISABLE_COPY(TaskDescription);
		};
	public:
		ThreadPool(size_t numThreads) {
			m_Threads.reserve(numThreads);
			for (size_t i = 0; i < numThreads; ++i) {
				m_Threads.emplace_back([this]() {
					InternalThreadLogic();
					});
			}
		}

		~ThreadPool() {
			m_IsBeingDestroyed = true;
			for (auto& thread : m_Threads) {
				thread.join();
			}
		}

		void RunTask(const std::function<void()>& task, std::mutex* mtx = nullptr) {
			m_Tasks.EmplaceBack(
				task,
				mtx == nullptr ? nullptr : std::make_unique<std::lock_guard<std::mutex>>(*mtx)
			);
		}

		void WaitUntilTasksExecuted(const std::vector<std::function<void()>>& tasks) {
			std::mutex mtx;
			std::unique_lock<std::mutex> lock(mtx);
			std::condition_variable cv;
			size_t tasksLeft = tasks.size();
			for (const auto& task : tasks) {
				RunTask([&]() {
					task();
					--tasksLeft;
					if (tasksLeft == 0) {
						cv.notify_one();
					}
				}, &mtx);
			}
			cv.wait(lock, [&]() { return tasksLeft == 0; });
		}

	private:
		void InternalThreadLogic() {
			while (!m_IsBeingDestroyed) {
				std::unique_ptr<TaskDescription> desc = m_Tasks.PeekAndPop();
				if (desc) {
					desc->TaskFunction();
				}
				else {
					std::this_thread::sleep_for(std::chrono::milliseconds(10)); // TODO CV
				}
			}
		}

	private:
		std::vector<std::thread> m_Threads;
		ThreadQueue<TaskDescription> m_Tasks;
		bool m_IsBeingDestroyed = false;
	};
}