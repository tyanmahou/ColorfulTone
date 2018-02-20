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
	constexpr double g_width = 252;

	//const Color ConvertColor(const Color& color)
	//{
	//	HSV hsv;
	//	hsv.convertFrom(color.r / 255.0, color.g / 255.0, color.b / 255.0);
	//	hsv.s = Min(0.5, hsv.s);
	//	hsv.v = 1.0;
	//	return hsv.toColor();
	//}

	double GetX(NoteType type)
	{
		switch (type % 10)
		{
		case 1:return 400 - g_width / 3;
		case 2:return 400;
		case 3:return 400 + g_width / 3;
		case 4:return 400 + g_width / 6;
		case 5:return 400;
		case 6:return 400 - g_width / 6;
		case 7:return 400;
		case 9:return 400;
		}
		return 400;
	}
	double GetY(double count, double scrollRate = 1.0, double speed = 1.0)
	{
		constexpr double adjust = 1.5;
		return  500 - count / Object::RESOLUTION * scrollRate*speed*adjust;
	}
	bool CanDraw(double y)
	{
		const double scale = Game::Instance()->m_config.m_playScale;
		return !(y <450 - 500.0 / scale || y>150.0 + 500.0 / scale);
		//return !(y <= -50 || y > 650);
	}

}


void Portrait::drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const
{
	const auto& config = Game::Instance()->m_config;

	Graphics2D::SetBlendState(BlendState::Subtractive);
	const double w = g_width *config.m_playScale;
	Rect(400 - w / 2, 0, w, 600).draw(ColorF(0.3, 0.6));
	Graphics2D::SetBlendState(BlendState::Default);


	{

		util::Transformer2D t2d(Mat3x2::Scale(config.m_playScale, Vec2{ 400,500 }));

		constexpr double height = 400;
		if (red)
		{
			constexpr Color color[4] = { ColorF(1,0,0,0),ColorF(1,0,0,0),Palette::White,Palette::White };
			Rect(400 - g_width / 2, 500 - height, g_width / 3, height).draw(color);
		}
		if (blue)
		{
			constexpr Color color[4] = { ColorF(0,0,1,0), ColorF(0,0,1,0),Palette::White,Palette::White };
			Rect(400 - g_width / 6, 500 - height, g_width / 3, height).draw(color);
		}
		if (yellow)
		{
			constexpr Color color[4] = { ColorF(1,1,0,0),ColorF(1,1,0,0),Palette::White,Palette::White };
			Rect(400 + g_width / 6, 500 - height, g_width / 3, height).draw(color);

		}


		this->drawJudgeLine();

		drawCallback();
		Graphics2D::SetBlendState(BlendState::Additive);

		PlayMusicGame::GetEffect(1).update();
		Graphics2D::SetBlendState(BlendState::Default);

	}
}

void Portrait::drawJudgeLine() const
{

	static constexpr Color judgeLineColor(255, 165, 0, 255);

	const double Y = GetY(0);
	//判定円
	Line({ 400 - g_width / 2,Y }, { 400 + g_width / 2,Y }).draw(15, judgeLineColor);

}

void Portrait::drawComboAndRate(int combo, float rate)
{
	const auto& font = FontAsset(L"combo");

	if (combo)
	{
		constexpr Vec2 pos{ 65 ,298};
		TextureAsset(L"combo").draw(pos.x + 121, pos.y+1, Palette::White);
		TextureAsset(L"combo").draw(pos.x + 120, pos.y, Palette::Black);
		font(Pad(combo, { 6,L' ' })).drawKinetic(pos.x + 1,pos.y+3, FontKinetic::DeleteSpace, Palette::White);
		font(Pad(combo, { 6,L' ' })).drawKinetic(pos.x, pos.y+2, FontKinetic::DeleteSpace, Palette::Black);

	}

	constexpr Vec2 pos{ 550 ,300 };

	font(L"{:.2f}%"_fmt, rate).draw(pos.x, pos.y+1, Palette::White);
	font(L"{:.2f}%"_fmt, rate).draw(pos.x, pos.y, Palette::Black);
}

void Portrait::drawTapEffect(int type)
{
	auto getWidth = [](int type) {
		switch (type % 10)
		{
		case 1:
		case 2:
		case 3:
			return g_width / 3.0;
			break;
		case 4:
		case 5:
		case 6:
			return g_width * 2 / 3.0;
		case 7:
			return g_width;
		}
		return g_width;
	};

	if (type % 10 == 5)
	{
		PlayMusicGame::GetEffect().add<TapEffect2>(GetX(1), getWidth(1));
		PlayMusicGame::GetEffect().add<TapEffect2>(GetX(3), getWidth(3));
		PlayMusicGame::GetEffect(1).add<TapEffect2_2>(400 - g_width / 2);
		PlayMusicGame::GetEffect(1).add<TapEffect2_2>(400 + g_width / 2);

	}
	else
	{
		PlayMusicGame::GetEffect(1).add<TapEffect2>(GetX(type), getWidth(type));
		PlayMusicGame::GetEffect(1).add<TapEffect2_2>(GetX(type));

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

	if(note.getSpeed()!=0)
	Line({ 400 - g_width / 2,y }, { 400 + g_width / 2,y }).draw(4, ColorF(0, 0.2));

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
		case 4:return Palette::Green;
		case 5:return Palette::Orange;
		case 6:return Palette::Purple;
		case 7:return Palette::Black;
		case 9:return Palette::Gray;
		}
		return Palette::Black;
	}

	const String& GetTextureName(NoteType type)
	{
		static const std::unordered_map<NoteType, String> textureNameMap
		{
			{ 1, L"portrait_notes1" },
			{ 2, L"portrait_notes2"	 },
			{ 3, L"portrait_notes3"	},
			{ 4, L"portrait_notes4"	},
			{ 5, L"portrait_notes5"	},
			{ 6, L"portrait_notes6"	},
			{ 7, L"portrait_notes7"	},
			{ 9, L"portrait_notes9"	},
			{ 10,L"portrait_notes10" },
			{ 11,L"portrait_notes1"	},
			{ 12,L"portrait_notes1"	},
			{ 13,L"portrait_notes3"	},
			{ 14,L"portrait_notes4"	},
			{ 15,L"portrait_notes5"	},
			{ 16,L"portrait_notes6"	},
			{ 17,L"portrait_notes7"	},
		};
		if (textureNameMap.count(type))
			return textureNameMap.at(type);

		return L"portrait_notes7";
	}
	void Draw(double y, NoteType type)
	{
		constexpr double h = 14;
		constexpr double w = g_width / 3;
		const auto& tex = TextureAsset(GetTextureName(type));
		const Color color = GetColor(type);
		switch (type)
		{
		case 1:
			tex.drawAt(400 - w, y)
				.drawFrame(0.5,0.5,color);
			break;
		case 2:
			tex.drawAt(400, y)
				.drawFrame(0.5, 0.5, color);
			break;
		case 3:
			tex.drawAt(400 + w, y)
				.drawFrame(0.5, 0.5, color);
			break;
		case 4:
			tex.scale(2, 1).drawAt(400 + w / 2, y)
				.drawFrame(0.5, 0.5, color);
			break;
		case 5:
			tex.scale(1.5, 1).drawAt(400 - w * 5 / 4, y)
				.drawFrame(0.5, 0.5, color);
			tex.scale(1.5, 1).drawAt(400 + w * 5 / 4, y)
				.drawFrame(0.5, 0.5, color);
			break;
		case 6:
			tex.scale(2.0, 1).drawAt(400 - w / 2, y)
				.drawFrame(0.5, 0.5, color);
			break;
		case 7:
			tex.scale(3.0, 1).drawAt(400, y)
				.drawFrame(0.5, 0.5, color);
			break;
		case 9:
			tex.scale(3.0, 1).drawAt(400, y)
				.drawFrame(0.5, 0.5, color);
			break;
		case 10:
			tex.scale(3.0, 1).drawAt(400, y)
				.drawFrame(0.5, 0.5,Palette::Black);
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
		DrawLong(400 - g_width / 2, pY, 21, y - pY, c1, c2);
		DrawLong(400 + g_width / 2, pY, 21, y - pY, c1, c2);
	}
	else
	{
		DrawLong(GetX(type), pY, 21, y - pY, c1, c2);
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


	Color c1 = HSV(static_cast<int>(count / 20) % 360, 0.5, 1);
	Color c2 = HSV((static_cast<int>(count / 20) + 72) % 360, 0.5, 1);

	DrawLong(400, pY, 140, y - pY, c1, c2);

}
