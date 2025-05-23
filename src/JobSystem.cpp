#include "JobSystem.h"

#include "Settings.h"
#include "Chunks/GenerateGrid.h"
#include "Chunks/CleanGrid.h"
#include "Chunks/GenerateGeometry.h"

namespace Vanadium {
	JobSystem::JobSystem(size_t threadCount) {
		for (size_t i = 0; i < threadCount; ++i) {
			m_Threads.emplace_back(&JobSystem::ThreadLoop, this);
		}
	}

	JobSystem::~JobSystem() {
		{
			std::unique_lock lock{ m_JobsMutex };
			m_ShouldTerminate = true;
			m_JobsConditionVar.notify_all();
		}

		for (auto& thread : m_Threads) {
			thread.join();
		}
	}

	void JobSystem::AddJob(const Job& j) {
		{
			std::unique_lock lock{ m_JobsMutex };

			m_Jobs.push_back(j);

			SortJobs();
		}

		m_JobsConditionVar.notify_one();
	}

	void JobSystem::AddJobs(const std::vector<Job>& jobs) {
		{
			std::unique_lock lock{ m_JobsMutex };

			m_Jobs.insert(m_Jobs.end(), jobs.begin(), jobs.end());

			SortJobs();
		}

		m_JobsConditionVar.notify_all();
	}

	void JobSystem::RemoveJob(const Job& j) {
		{
			std::unique_lock lock{ m_JobsMutex };

			auto it = std::find_if(m_Jobs.begin(), m_Jobs.end(), [&](const Job& job) { return job.position == j.position; });
			if (it != m_Jobs.end()) {
				m_Jobs.erase(it);
			}

			SortJobs();
		}
	}

	void JobSystem::ClearJobs() {
		{
			std::unique_lock lock{ m_JobsMutex };
			
			m_Jobs.clear();
		}
	}

	std::vector<Chunk> JobSystem::GetResults() {
		std::vector<Chunk> results;
		{
			std::unique_lock lock{ m_ResultsMutex };

			results = std::move(m_Results);
		}
		
		return results;
	}

	void JobSystem::WaitForCompletion() {
		{
			std::unique_lock lock{ m_JobsMutex };
			m_AllJobsCompleted.wait(lock, [this] { return m_Jobs.empty() && m_JobsInProgress == 0; });
		}
	}

	void JobSystem::ThreadLoop() {
		while (true) {
			Job job{ };

			{
				std::unique_lock lock{ m_JobsMutex };

				m_JobsConditionVar.wait(lock, [this] { return !m_Jobs.empty() || m_ShouldTerminate; });

				if (m_ShouldTerminate) {
					return;
				}

				if (m_Jobs.empty()) {
					continue; // Spurious Wakeup
				}

				job = m_Jobs.back();
				m_Jobs.pop_back();

				m_JobsInProgress++;
			}

			Chunk chunk{ };

			chunk.position = job.position;

			chunk.grid = CreateGrid(chunk.position, job.chunkSize, job.settings);

			Grid pxGrid = CreateGrid(chunk.position + ChunkPosition{ 1, 0, 0 }, job.chunkSize, job.settings);
			Grid pyGrid = CreateGrid(chunk.position + ChunkPosition{ 0, 1, 0 }, job.chunkSize, job.settings);
			Grid pzGrid = CreateGrid(chunk.position + ChunkPosition{ 0, 0, 1 }, job.chunkSize, job.settings);
			Grid nxGrid = CreateGrid(chunk.position + ChunkPosition{ -1, 0, 0 }, job.chunkSize, job.settings);
			Grid nyGrid = CreateGrid(chunk.position + ChunkPosition{ 0, -1, 0 }, job.chunkSize, job.settings);
			Grid nzGrid = CreateGrid(chunk.position + ChunkPosition{ 0, 0, -1 }, job.chunkSize, job.settings);

			chunk.grid = CleanGrid(
				chunk.grid, 
				job.chunkSize,
				pxGrid,
				pyGrid,
				pzGrid,
				nxGrid,
				nyGrid,
				nzGrid
			);

			chunk.geometry = GenerateGeometry(
				chunk.position, 
				chunk.grid, 
				job.chunkSize,
				pxGrid,
				pyGrid,
				pzGrid,
				nxGrid,
				nyGrid,
				nzGrid
			);

			chunk.remakeChunk = false;

			{
				std::unique_lock lock{ m_ResultsMutex };

				m_Results.push_back(std::move(chunk));
			}

			{
				std::unique_lock lock{ m_JobsMutex };
				--m_JobsInProgress;

				if (m_Jobs.empty() && m_JobsInProgress == 0) {
					m_AllJobsCompleted.notify_all();
				}
			}
		}
	}

	void JobSystem::SortJobs() {
		std::sort(m_Jobs.begin(), m_Jobs.end(), [](const Job& j1, const Job& j2) { return j1.priority < j2.priority; });
	}
}