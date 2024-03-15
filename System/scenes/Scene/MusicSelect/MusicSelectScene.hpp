#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/MusicSelect/MusicSelectSceneView.hpp>
#include <scenes/Scene/MusicSelect/SelectMusicsInfo.hpp>
#include <memory>
#include <Siv3D/Types.hpp>
#include <Siv3D/Array.hpp>

namespace ct
{
	class HighSpeedDemo;
	class ConfigMain;

	class MusicSelectScene : public ISceneBase
	{
	public:
		enum class Action : s3d::uint8
		{
			GenreSelect,
			MusicSelect,
			LevelSelect,
		};
	private:
		class Model;
		std::shared_ptr<Model> m_pModel;
		MusicSelectSceneView m_view;
	public:
		MusicSelectScene(const InitData& init);
		~MusicSelectScene() = default;

		void finally() override;
		void update() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		//get current select info
		static SelectMusicsInfo GetSelectInfo();

		const s3d::Array<MusicData>& getMusics()const;

		Action getAction()const;

		// previous , current
		std::pair<Action, Action> getChangeAction()const;
		s3d::int32 getMoveSelect()const;

		const HighSpeedDemo& getHighSpeedDemo()const;
		double getScrollRate() const
		{
			return getData().m_scrollRate;
		}
		const ConfigMain& getConfig() const;
	};
}
