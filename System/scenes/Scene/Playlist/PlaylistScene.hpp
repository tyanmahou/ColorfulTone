#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/Playlist/PlaylistSceneView.hpp>

namespace ct
{
	class HighSpeedDemo;
	class ConfigMain;

	class PlaylistScene :public ISceneBase
	{
	public:
		enum class MemoInfo : bool
		{
			Notes,
			Other,
		};
	private:
		class Model;
		std::shared_ptr<Model> m_pModel;
		PlaylistSceneView m_view;
	public:
		PlaylistScene(const InitData& init);

		void update() override;
		void finally() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		const s3d::Optional<s3d::String>& title() const;

		const PlaySession& getPlay() const;
		const HighSpeedDemo& getHighSpeedDemo()const;
		const ConfigMain& getConfig()const;

		double getScrollRate()const
		{
			return getData().m_scrollRate;
		}

		static MemoInfo GetMemoInfo();
	};
}
