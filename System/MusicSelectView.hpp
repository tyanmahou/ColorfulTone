#pragma once
#include<memory>

class MusicSelect;

class MusicSelectView
{
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	MusicSelectView(const MusicSelect*const scene);
	~MusicSelectView();

	void resetShaderTimer();
	void onChangeAction();
	void update();

	void draw()const;
};

