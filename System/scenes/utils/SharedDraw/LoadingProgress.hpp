#pragma once
#include <Siv3D/String.hpp>
#include <Siv3D/Stopwatch.hpp>

namespace ct::SharedDraw
{
	class LoadingProgress
	{
	public:
		LoadingProgress(const s3d::String& text = U"NOW LOADING...");

		void onCompleted();
		void update();
		void draw(double progress)const;
		s3d::int32 getStopwatchMs() const;

	private:
		s3d::String m_message;
		s3d::Stopwatch m_stopwatch;
		bool m_isCompleted;
	};
}