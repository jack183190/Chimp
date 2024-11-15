#pragma once

#include "stdafx.h"
#include "api/utils/ThreadQueue.h"

namespace Chimp {
	// Create a thread pool that can execute tasks asynchronously
	// wip tasks will be discarded upon destruction of the thread pool
	// NumThreads - the number of threads to use
	template <size_t NumThreads>
	class ThreadPool {
	public:
		typedef std::function<void()> Task;
		struct TaskDescription {
			Task TaskFunction; // This is the actual task
			std::unique_ptr<std::lock_guard<std::mutex>> Lock; // Lock a mutex while the task is running (may be nullptr)
			TaskDescription(TaskDescription&& other) {
				// move
				TaskFunction = std::move(other.TaskFunction);
				Lock = std::move(other.Lock);
			}
		};
	public:
		ThreadPool() {
			for (size_t i = 0; i < NumThreads; ++i) {
				m_Threads[i] = std::thread([this]() {
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
				mtx == nullptr ? nullptr : std::make_unique<std::lock_guard<std::mutex>(*mtx)>()
			);
		}

	private:
		void InternalThreadLogic() {
			while (!m_IsBeingDestroyed) {
				TaskDescription desc = m_Tasks.PeekAndPop();
				desc.TaskFunction();
			}
		}

	private:
		std::array<std::thread, NumThreads> m_Threads;
		ThreadQueue<TaskDescription> m_Tasks;
		bool m_IsBeingDestroyed = false;
	};
}