#pragma once
#include <core/Play/PlayStyle/PlayStyle.hpp>

namespace ct
{
	// 通常 アーク
	class NormalArcStyle : public IPlayStyle
	{
		void drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const override;
		void drawJudgeLine()const override;
		void drawComboAndRate(size_t combo, float rate);

		void drawTapEffect(NoteType type, NoteType baseType);
		void drawJudgeEffect(const s3d::String& str, NoteType type);

		void draw(const Bar& note, double count, double scrollRate)const override;

		void draw(const Note& note, double count, double scrollRate)const override;

		void draw(const LongNote& note, double count, double scrollRate)const override;

		void draw(const RepeatNote& note, double count, double scrollRate)const override;

		void draw(const RepeatEnd& note, double count, double scrollRate)const override;

	private:
		s3d::Effect m_effetcs[2];
	};
}

