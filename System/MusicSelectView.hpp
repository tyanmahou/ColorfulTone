#pragma once

class MusicSelect;

class MusicSelectView
{
	const MusicSelect*const m_pScene;
public:
	MusicSelectView(const MusicSelect*const scene);
	~MusicSelectView();

	void draw()const;
};

