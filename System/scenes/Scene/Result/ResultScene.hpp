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
		const ScoreModel& getResult()const;
		const PlayingScore& getScore() const;
		const s3d::Optional<CourseResult>& getCourseResult() const;
		const s3d::Optional<EndlessResult>& getEndlessResult() const;

		bool isNewRecord() const;

		const s3d::Optional<s3d::String>& playlistName() const;

	private:
		class Model;
		std::shared_ptr<Model> m_model;

		ResultSceneView m_view;
	};
}
