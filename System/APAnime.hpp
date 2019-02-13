#pragma once
#include<memory>

struct Score;

class APAnime
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	APAnime();
	~APAnime();
	bool isStart() const;
	bool isEnd() const;
	void play(const Score& score) const;
	void update() const;
	void draw()const;
};

