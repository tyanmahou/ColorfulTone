#include"Portrait.h"

#include"Note.h"
#include"Bar.h"
#include"LongNote.h"
#include"RepeatNote.h"
#include"Util.h"
#include"PlayMusicGame.h"
#include"TapEffect.h"
#include"JudgeEffect.h"
#include"FontKinetic.h"

namespace
{
	const Color ConvertColor(const Color& color)
	{
		HSV hsv;
		hsv.convertFrom(color.r / 255.0, color.g / 255.0, color.b / 255.0);
		hsv.s = Min(0.5, hsv.s);
		hsv.v = 1.0;
		return hsv.toColor();
	}

	double GetX(NoteType type)
	{
		switch (type % 10)
		{
		case 1:return 400 - 120;
		case 2:return 400;
		case 3:return 400 + 120;
		case 4:return 400 + 60;
		case 5:return 400;
		case 6:return 400 - 60;
		case 7:return 400;
		case 9:return 400;
		}
		return 400;
	}
	double GetY(double count, double scrollRate = 1.0, double speed = 1.0)
	{
		constexpr double adjust = 1.7;
		return  500 - count / Object::RESOLUTION * scrollRate*speed*adjust;
	}
	bool CanDraw(double y)
	{
		return !(y <= -50 || y > 650);
	}

}


void Portrait::drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const
{
	Graphics2D::SetBlendState(BlendState::Subtractive);
	Rect(400 - 182, 0, 364, 600).draw(ColorF(0.3, 0.3));
	Graphics2D::SetBlendState(BlendState::Default);

	constexpr double height = 400;
	if (red)
	{
		constexpr Color color[4] = { ColorF(1,0,0,0),ColorF(1,0,0,0),Palette::White,Palette::White };
		Rect(400 - 180, 600 - height, 120, height).draw(color);
	}
	if (blue)
	{
		constexpr Color color[4] = { ColorF(0,0,1,0), ColorF(0,0,1,0),Palette::White,Palette::White };
		Rect(400 - 60, 600 - height, 120, height).draw(color);
	}
	if (yellow)
	{
		constexpr Color color[4] = { ColorF(1,1,0,0),ColorF(1,1,0,0),Palette::White,Palette::White };
		Rect(400 + 60, 600 - height, 120, height).draw(color);

	}
	//Graphics2D::SetBlendState(BlendState::Default);


	this->drawJudgeLine();

	drawCallback();

}

void Portrait::drawJudgeLine() const
{

	static constexpr Color judgeLineColor(255, 165, 0, 255);

	const double Y = GetY(0);
	//判定円
	Line({ 400 - 180,Y }, { 400 + 180,Y }).draw(4, judgeLineColor);

}

void Portrait::drawComboAndRate(int combo, float rate)
{
	const auto& font = FontAsset(L"combo");

	if (combo)
	{
		const int x = 115;
		TextureAsset(L"combo").draw(x + 121, 299, Palette::White);
		TextureAsset(L"combo").draw(x + 120, 298, Palette::Black);
		font(Pad(combo, { 6,L' ' })).drawKinetic(x + 1, 301, FontKinetic::DeleteSpace, Palette::White);
		font(Pad(combo, { 6,L' ' })).drawKinetic(x, 300, FontKinetic::DeleteSpace, Palette::Black);

	}

	font(L"{:.2f}%"_fmt, rate).draw(501, 301, Palette::White);
	font(L"{:.2f}%"_fmt, rate).draw(500, 300, Palette::Black);
}

void Portrait::drawTapEffect(int type)
{
	auto getWidth = [](int type) {
		switch (type % 10)
		{
		case 1:
		case 2:
		case 3:
			return 120;
			break;
		case 4:
		case 5:
		case 6:
			return 240;
		case 7:
			return 360;
		}
		return 360;
	};

	if (type % 10 == 5)
	{
		PlayMusicGame::GetEffect().add<TapEffect2>(GetX(1), getWidth(1));
		PlayMusicGame::GetEffect().add<TapEffect2>(GetX(3), getWidth(3));
	}
	else
	{
		PlayMusicGame::GetEffect().add<TapEffect2>(GetX(type), getWidth(type));
	}

}

void Portrait::drawJudgeEffect(const String & str, int type)
{
	PlayMusicGame::GetEffect().add<JudgeEffect>(str, Vec2{ GetX(type),400 });
}

void Portrait::draw(const Bar & note, double count, float scrollRate) const
{
	double y = GetY(count, scrollRate, note.getSpeed());


	if (!CanDraw(y))
		return;

	Line({ 400 - 180,y }, { 400 + 180,y }).draw(4, ColorF(0, 0.2));

}

namespace
{
	Color GetColor(NoteType type)
	{
		switch (type)
		{
		case 1:return Palette::Red;
		case 2:return Palette::Blue;
		case 3:return Palette::Yellow;
		case 4:return Palette::Greenyellow;
		case 5:return Palette::Orange;
		case 6:return Palette::Purple;
		case 7:return Palette::Black;
		case 9:return Palette::White;
		}
		return Palette::Black;
	}
	void Draw(double y, NoteType type)
	{
		constexpr double h = 10;
		const Color color = GetColor(type);
		switch (type)
		{
		case 1:
			Rect({ 400 - 180,y - h / 2 }, { 120,h }).draw(color).drawFrame(1, 1, Color(255, 115, 115));
			break;
		case 2:
			Rect({ 400 - 60,y - h / 2 }, { 120,h }).draw(color).drawFrame(1, 1, Color(115, 115, 255));

			break;
		case 3:
			Rect({ 400 + 60,y - h / 2 }, { 120,h }).draw(color).drawFrame(1, 1, Color(140, 140, 0));

			break;
		case 4:
			Rect({ 400 - 60,y - h / 2 }, { 240,h }).draw(color).drawFrame(1, 1, Color(0, 140, 0));
			break;
		case 5:
			Rect({ 400 - 180,y - h / 2 }, { 120,h }).draw(color).drawFrame(1, 1, Color(140, 70, 0));
			Rect({ 400 + 60,y - h / 2 }, { 120,h }).draw(color).drawFrame(1, 1, Color(140, 70, 0));
			break;
		case 6:
			Rect({ 400 - 180,y - h / 2 }, { 240,h }).draw(color).drawFrame(1, 1, Color(255, 115, 255));
			break;
		case 7:
			Rect({ 400 - 180,y - h / 2 }, { 360,h }).draw(color).drawFrame(1, 1);
			break;
		case 9:
			Rect({ 400 - 180,y - h / 2 }, { 360,h }).draw(color).drawFrame(1, 1, Palette::Black);
			break;
		case 10:
			Rect({ 400 - 180,y - h / 2 }, { 360,h }).draw({ Palette::Red, ColorF(0,1,0),Palette::Blue ,Palette::White }).drawFrame(1, 1, Palette::Black);
		default:
			break;
		}
	}

	void DrawLong(double x, double y, double w, double h, const Color& c1, const Color& c2)
	{
		Rect(x - w / 2, y, w, h).draw(ColorF(0, 0.5));
		Rect(x - w / 2 + 5, y, w - 10, h + 5).draw({ c1,c1,c2,c2 });
	}
}
void Portrait::draw(const Note & note, double count, float scrollRate) const
{

	const NoteType type = note.getType();

	if (type >= 11 && note.isFirstTap())
		count = 0;

	double y = GetY(count, scrollRate, note.getSpeed());

	if (!CanDraw(y))
		return;

	Draw(y, type % 10);
}


void Portrait::draw(const LongNote & note, double count, float scrollRate) const
{
	const auto parent = note.getParent();

	const NoteType type = note.getType();

	const Color& color = note.getColor();

	double y = GetY(count, scrollRate, note.getSpeed());


	const double nowCount = note.getDrawCount() - count;
	auto pCount = parent->getDrawCount() - nowCount;
	if (parent->isFirstTap())
		pCount = 0;

	if (pCount > count)
		return;

	double pY = GetY(pCount, scrollRate, parent->getSpeed());


	if (!CanDraw(pY) && !CanDraw(y))
		return;


	Color c1 = color;
	Color c2 = GetColor(type % 10);


	if (type % 10 == 5)
	{
		DrawLong(GetX(1), pY, 30, y - pY, c1, c2);
		DrawLong(GetX(3), pY, 30, y - pY, c1, c2);
	}
	else
	{
		DrawLong(GetX(type), pY, 30, y - pY, c1, c2);
	}
}

void Portrait::draw(const RepeatNote & note, double count, float scrollRate) const
{
	if (note.isFirstTap())
		count = 0;

	double y = GetY(count, scrollRate, note.getSpeed());

	if (!CanDraw(y))
		return;


	Draw(y, 10);
}

void Portrait::draw(const RepeatEnd & note, double count, float scrollRate) const
{
	const auto parent = note.getParent();

	double y = ::GetY(count, scrollRate, note.getSpeed());

	const double nowCount = note.getDrawCount() - count;
	auto pCount = parent->getDrawCount() - nowCount;
	if (parent->isFirstTap())
		pCount = 0;
	auto pY = GetY(pCount, scrollRate, parent->getSpeed());

	if (!CanDraw(pY) && !CanDraw(y))
		return;


	Color c1 = HSV(static_cast<int>(count / 10) % 360, 0.5, 1);
	Color c2 = HSV((static_cast<int>(count / 10) + 72) % 360, 0.5, 1);

	DrawLong(400, pY, 200, y - pY, c1, c2);

}
