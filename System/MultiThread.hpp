#pragma once
#include <unordered_map>
#include <future>
#include <mutex>
#include<Siv3D/String.hpp>

class MultiThread final
{
	using Tasks = std::unordered_map<s3d::String, std::future<void>>;
private:
	Tasks m_tasks;
	std::mutex m_mutex;

	MultiThread() = default;
	MultiThread(const MultiThread&) = delete;
	MultiThread& operator = (const MultiThread&) = delete;

	template<class Func, class... Params>
	void async(const s3d::String& key, Func task, Params&& ...args)
	{
		m_tasks.insert_or_assign(key, std::async(std::launch::async, task, std::forward<Params>(args)...));
	}

	void update()
	{
		for (auto&& task : m_tasks) {
			task.second.get();
		}
	}

	static MultiThread& Get()
	{
		static MultiThread instance;
		return instance;
	}
public:
	template<class Func, class... Params>
	static void Async(const s3d::String& key, Func task, Params&& ...args)
	{
		Get().async(key, task, std::forward<Params>(args)...);
	}

	static void Wait(const s3d::String& key)
	{
		const auto & tasks = Get().m_tasks;
		if (tasks.find(key) != tasks.end()) {
			tasks.at(key).wait();
		}
	}

	static void Update()
	{
		Get().update();
	}

	static std::mutex& GetMutex()
	{
		return Get().m_mutex;
	}
};
