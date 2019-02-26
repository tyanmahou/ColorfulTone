#pragma once
#include<memory>

#include"ISceneBase.hpp"
#include"ResultSceneView.hpp"

class ResultScene :public ISceneBase
{
private:
	class Model;
	std::shared_ptr<Model> m_model;

	ResultSceneView m_view;
public:
	ResultScene();
	~ResultScene() = default;

	void init()override;
	void finally()override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

	const NotesData& getNotes() const;
	const Score& getResult()const;
	const ScoreModel& getScore() const;
	bool isNewRecord()const;
};