#pragma once
#include<memory>

class ResultScene;

class ResultSceneView
{
private:
	class Impl;
	std::shared_ptr<Impl> m_impl;
public:
	ResultSceneView(const ResultScene* const scene);
	~ResultSceneView() = default;

	void init();
	void update() const;
	void draw() const;
};

