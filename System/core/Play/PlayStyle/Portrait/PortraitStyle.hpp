#pragma once
#include <core/Play/PlayStyle/PlayStyle.hpp>

namespace ct
{
	// 縦レーンモード
	class PortraitStyle : public IPlayStyle
	{
	protected:
		void drawPreview([[maybe_unused]] std::function<void()> drawCallback) const override;
		void drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const override;
		void drawJudgeLine()const override;
		void drawComboAndRate(size_t combo, float rate, const s3d::Optional<float>& subRate);

		void drawTapEffect(NoteType type, NoteType baseType);
		void drawJudgeEffect(Score::Judge judge, NoteType type, s3d::int64 diff);

		void draw(const Bar& note, double count, double scrollRate)const override;

		void draw(const Note& note, double count, double scrollRate)const override;

		void draw(const LongNote& note, double count, double scrollRate)const override;

		void draw(const RepeatNote& note, double count, double scrollRate)const override;

		void draw(const RepeatEnd& note, double count, double scrollRate)const override;

		virtual bool canDraw(double y) const;
		void drawComboAndRate(size_t combo, const s3d::Vec2& comboPos, float rate, const s3d::Optional<float>& subRate, const s3d::Vec2& ratePos) const;
	protected:
		s3d::Effect m_effetcs[2];
	};
}