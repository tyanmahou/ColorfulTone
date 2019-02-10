#pragma once
#include<Siv3D/EasingController.hpp>
class MusicSelect;

class MusicSelectView
{
	const MusicSelect*const m_pScene;

	double m_shaderTimer = 0.0;
	s3d::EasingController<double> m_memoOffset;
public:
	MusicSelectView(const MusicSelect*const scene);
	~MusicSelectView();

	void resetShaderTimer();
	void onChangeAction();
	void update();

	void draw()const;
};

