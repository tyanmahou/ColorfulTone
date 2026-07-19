#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/Endless/EndlessSceneView.hpp>

namespace ct
{
	class HighSpeedDemo;
	class ConfigMain;

	class EndlessScene :public ISceneBase
	{
	public:
		enum class MemoInfo : bool
		{
			Endless,
			Notes,
		};
	private:
		class Model;
		std::shared_ptr<Model> m_pModel;
		EndlessSceneView m_view;
	public:
		EndlessScene(const InitData& init);

		void update() override;
		void finally() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		const PlayEndless& getPlay() const;
		const HighSpeedDemo& getHighSpeedDemo()const;
		const ConfigMain& getConfig()const;

		double getScrollRate()const
		{
			return getData().m_scrollRate;
		}

		static MemoInfo GetMemoInfo();
	};
}
