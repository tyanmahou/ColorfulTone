﻿#pragma once
#include <core/Play/PlayStyle/PlayStyle.hpp>

namespace ct
{
	// 横レーンモード
	class LandscapeStyle : public IPlayStyle
	{
	protected:
		void drawPreview(std::function<void()> drawCallback) const override;
		void drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const override;
		void drawJudgeLine()const override;
		void drawComboAndRate(size_t combo, float rate);

		void drawTapEffect(NoteType type, NoteType baseType);
		void drawJudgeEffect(Score::Judge judge, NoteType type, s3d::int64 diff);

		void draw(const Bar& note, double count, double scrollRate)const override;

		void draw(const Note& note, double count, double scrollRate)const override;

		void draw(const LongNote& note, double count, double scrollRate)const override;

		void draw(const RepeatNote& note, double count, double scrollRate)const override;

		void draw(const RepeatEnd& note, double count, double scrollRate)const override;
	protected:
		s3d::Effect m_effetcs[2];
	};
}