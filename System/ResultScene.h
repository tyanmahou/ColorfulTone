#pragma once

#include"ISceneBase.hpp"

#include"ResultTweet.h"
#include"SerialImage.h"

class ResultScene :public ISceneBase
{

private:
	enum class PassEffect
	{
		Pass,
		Failure,
		None
	}m_passEffect= PassEffect::None;
	SerialImage m_backLight;
	bool m_isPassEffectEnd = false;
	EasingController<double> m_passEffectEasing;

	int m_timer;				//タイマー
	Font m_font;
	Font m_artistFont;
	Font m_nameFont;
	Font m_rateFont;

	Score m_resultScore;

	unsigned int m_notesSum;	//ノーツ数
	float  m_clearRate;			//クリア率
	SpecialResult m_specialResult;	//フルコンやＡＰの情報

	bool m_isClear;
	bool m_isNewRecord;
	bool m_startEffect;
	//演出用
	Effect m_effect;
	std::array<EasingController<int>, Score::Judge::TERM> m_judgeEasing;
	EasingController<double> m_rateEasing;
	EasingController<int> m_maxComboEasing,m_totalNotesEasing;

	ResultTweet m_tweet;
public:


	ResultScene();
	~ResultScene();

	void init()override;
	void update() override;
	void draw() const override;
	void drawFadeIn(double t) const override;
	void drawFadeOut(double t) const override;
};
