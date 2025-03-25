#include "JobSystem.h"

#include "Settings.h"
#include "Chunks/GenerateChunk.h"
#include "Chunks/GenerateGrid.h"
#include "Chunks/CleanGrid.h"
#include "Chunks/GenerateGeometry.h"

namespace Vanadium {
	JobSystem::JobSystem(size_t threadCount) {
		std::cout << "Creating job system" << std::endl;
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

		std::cout << "Closed job system" << std::endl;
	}

	void JobSystem::AddJob(const Job& j) {
		{
			std::unique_lock lock{ m_JobsMutex };

			m_Jobs.push_back(j);

			SortJobs();
		}

		m_JobsConditionVar.notify_one();
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

			int n = 16; // TODO
			chunk.grid = Vanadium::CreateGrid(chunk.position, n, Settings{ }); // TODO settings
			chunk.grid = Vanadium::CleanGrid(chunk.grid, n);
			chunk.geometry = Vanadium::GenerateGeometry(chunk.position, chunk.grid, n, 2, 2);

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

	}
}