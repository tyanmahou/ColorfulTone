#pragma once
#include <memory>

namespace ct
{
	struct PlayingScore;

	class ApAnime
	{
	public:
		ApAnime();
		~ApAnime();
		bool isStart() const;
		bool isEnd() const;
		void play(const PlayingScore& score) const;
		void update() const;
		void draw()const;

	private:
		class Impl;
		std::unique_ptr<Impl> m_pImpl;
	};
}
