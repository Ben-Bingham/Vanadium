#pragma once

#include <vector>

#include "Chunks/Chunk.h"
#include "Settings.h"

namespace Vanadium {
	struct Job {
		ChunkPosition position{ };

		Settings settings{ };

		int chunkSize{ 8 };

		size_t priority{ 0 }; // Higher is more urgent
	};

	class JobSystem {
	public:
		JobSystem(size_t threadCount);
		JobSystem(const JobSystem& other) = delete;
		JobSystem(JobSystem&& other) noexcept = default;
		JobSystem& operator=(const JobSystem& other) = delete;
		JobSystem& operator=(JobSystem&& other) noexcept = default;
		~JobSystem();

		void AddJob(const Job& j);

		std::vector<Chunk> GetResults();

		void WaitForCompletion();

	private:
		void ThreadLoop();

		void SortJobs();

		std::mutex m_JobsMutex;
		std::condition_variable m_JobsConditionVar;
		std::vector<Job> m_Jobs;

		std::mutex m_ResultsMutex;
		std::vector<Chunk> m_Results;

		std::vector<std::thread> m_Threads;

		bool m_ShouldTerminate{ false };

		std::atomic<int> m_JobsInProgress{ 0 };

		std::condition_variable m_AllJobsCompleted;
	};
}