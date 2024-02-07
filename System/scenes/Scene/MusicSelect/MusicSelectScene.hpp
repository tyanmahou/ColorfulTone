#pragma once
#include <scenes/Scene/ISceneBase.hpp>
#include <scenes/Scene/MusicSelect/MusicSelectSceneView.hpp>
#include <memory>
#include <Siv3D/Types.hpp>
#include <Siv3D/Array.hpp>

namespace ct
{
	class HighSpeedDemo;
	class ConfigMain;

	using MusicDataRef = std::reference_wrapper<MusicData>;

	class MusicSelectScene : public ISceneBase
	{
	public:
		enum class SortMode : s3d::uint8
		{
			FileName,
			MusicName,
			ArtistName,
			LastUpdateAt,
			Default = FileName
		};
		enum class AllNotesInfo : bool
		{
			Level,
			ClearRank
		};
		struct SelectMusicsInfo
		{
			s3d::uint32 genre = 0;	// 選択中のジャンル
			s3d::uint32 music = 0;	// 選択中の曲
			s3d::uint32 level = 0;	// 選択中のレベル
			SortMode sortMode = SortMode::Default; // 選択中のソート
			AllNotesInfo notesInfo = AllNotesInfo::Level; //選択中の譜面表示情報
		};
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

		const s3d::Array<MusicDataRef>& getMusics()const;

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
