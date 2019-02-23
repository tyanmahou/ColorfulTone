#pragma once
#include"ISceneBase.hpp"
#include"MusicSelectView.hpp"
#include<memory>

class HighSpeedDemo;

class MusicSelect :public ISceneBase
{
public:
	enum class SortMode : uint8
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
		uint32 genre = 0;	// 選択中のジャンル
		uint32 music = 0;	// 選択中の曲
		uint32 level = 0;	// 選択中のレベル
		SortMode sortMode = SortMode::Default; // 選択中のソート
		AllNotesInfo notesInfo = AllNotesInfo::Level; //選択中の譜面表示情報
	};
	enum class Action :uint8
	{
		GenreSelect,
		MusicSelect,
		LevelSelect,
	};
private:
	class Model;
	std::shared_ptr<Model> m_pModel;
	MusicSelectView m_view;

public:
	MusicSelect();
	~MusicSelect() = default;

	void init() override;
	void finally() override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;

	//get current select info
	static SelectMusicsInfo GetSelectInfo();

	const Array<MusicData>& getMusics()const;

	Action getAction()const;

	// previous , current
	std::pair<Action, Action> getChangeAction()const;
	int getMoveSelect()const;

	const HighSpeedDemo& getHighSpeedDemo()const;
	float getScrollRate()const
	{
		return m_data->m_scrollRate;
	}
};

