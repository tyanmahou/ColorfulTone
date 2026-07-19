#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/EndlessSelect/EndlessSelectSceneView.hpp>

namespace ct
{
	class ConfigMain;
	class EndlessSelectScene : public ISceneBase
	{
	public:
		struct SelectInfo
		{
			s3d::uint32 endless = 0; // 選択中のエンドレス
		};
	private:

		class Model;
		std::shared_ptr<Model> m_pModel;
		EndlessSelectSceneView m_view;
	public:
		EndlessSelectScene(const InitData& init);
		~EndlessSelectScene() = default;

		void update() override;
		void finally() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		static SelectInfo GetSelectInfo();

		const Array<EndlessData>& getEndless() const;

		// previous , current
		s3d::int32 getMoveSelect() const;
		const ConfigMain& getConfig() const;
	};
}
