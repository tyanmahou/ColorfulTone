#pragma once
#include <memory>
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/Result/ResultSceneView.hpp>

namespace ct
{
	class ResultScene : public ISceneBase
	{
	public:
		ResultScene(const InitData& init);
		~ResultScene() = default;

		void finally()override;
		void update() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		const NotesData& getNotes() const;
		const Score& getResult()const;
		const ScoreModel& getScore() const;
		bool isNewRecord()const;

		const PlayCourse& getPlayCourse()const;
	private:
		class Model;
		std::shared_ptr<Model> m_model;

		ResultSceneView m_view;
	};
}
