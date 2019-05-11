#pragma once

#include<memory>

class DownloadScene;

class DownloadSceneView
{
private:
	class Impl;
	std::shared_ptr<Impl> m_pImpl;
public:
	DownloadSceneView(const DownloadScene* const scene);
	~DownloadSceneView() = default;

	void draw()const;
};