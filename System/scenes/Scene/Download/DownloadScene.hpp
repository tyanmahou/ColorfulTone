#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/Download/DownloadSceneView.hpp>

namespace ct
{
	class DownloadContent;

	class DownloadScene : public ISceneBase
	{
	public:
		struct SelectContent
		{
			s3d::uint32 content = 0; // 選択中のコンテンツ
		};
	private:
		class Model;
		std::shared_ptr<Model> m_pModel;
		DownloadSceneView m_view;
	public:
		DownloadScene(const InitData& init);
		~DownloadScene() = default;

		void update() override;
		void finally() override;
		void draw() const override;
		void drawFadeIn(double t) const override;
		void drawFadeOut(double t) const override;

		const Array<DownloadContent>& getContents()const;

		s3d::int32 getMoveSelect()const;
		bool isListLoading()const;
		bool isDownloading()const;
		double getProgress()const;

		static SelectContent GetSelectInfo();
	};
}
