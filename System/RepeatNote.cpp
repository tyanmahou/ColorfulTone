#include"RepeatNote.h"
#include"TapEffect.h"
#include"AutoPlayManager.h"
#include"PlayMusicGame.h"
namespace
{
	void EffectUpdate(bool red, bool blue, bool yellow)
	{
		if (red)
			PlayMusicGame::GetEffect().add<TapEffect>(7 * Pi / 6, 1);
		if (blue)
			PlayMusicGame::GetEffect().add<TapEffect>(Pi / 2, 2);
		if (yellow)
			PlayMusicGame::GetEffect().add<TapEffect>(11 * Pi / 6, 3);
		SoundManager::SE::Play(L"PERFECT");
	}

	const Color ConvertColor(const Color& color)
	{
		HSV hsv;
		hsv.convertFrom(color.r / 255.0, color.g / 255.0, color.b / 255.0);
		hsv.s = Min(0.5, hsv.s);
		hsv.v = 1.0;
		return hsv.toColor();
	}

}

double RepeatEnd::notesTapCount = 0;

bool RepeatEnd::update(double & nowCount, double & countPerFrame, Score & score, Sound & sound)
{
	if (!m_isActive || !m_parent->isFirstTap())
		return true;

	auto count = m_count - nowCount;

	//初期化
	if (!m_isStart)
	{
		m_isStart = true;
		m_lastCount = nowCount;
	}
	//オートプレイ----------------------
	if (AutoPlayManager::Instance()->m_autoPlay)
	{
		if (m_lastCount == nowCount||nowCount > m_lastCount + NotesData::RESOLUTION / (m_interval * 2))
		{
			static int tap = 0;
			++tap %= 3;
			EffectUpdate(tap == 0, tap == 1, tap == 2);
			m_lastCount = nowCount;
			m_isTap = true;
		}

	}
	else
		//----------------------------------
		if (m_judge())
		{
			EffectUpdate(PlayKey::Red().clicked, PlayKey::Blue().clicked, PlayKey::Yellow().clicked);
			m_lastCount = nowCount;
			m_isTap = true;
		}

	if (notesTapCount > m_lastCount)
	{
		m_lastCount = notesTapCount;
		m_isTap = true;
	}


	if (count <= 0)//ロングの終点
	{
		if (m_isTap)
			perfect(score);
		else
			miss(score);

		return false;
	}
	//8分間隔以内で連打しないとミス
	if (nowCount > m_lastCount + NotesData::RESOLUTION / m_interval)
	{
		auto aCount = Abs(count);
		//パーフェクト範囲内での話はセーフ
		if (aCount <= JudgeRange(countPerFrame, Judge::Perfect) && m_isTap)
			perfect(score);
		else
			miss(score);
	}
	return false;
}

void RepeatEnd::diffDraw(double count, float scrollRate) const
{
	auto angle = m_parent->getAngle();
	Vec2 pos;
	pos.x = 400 + 40 * cos(angle) + (count / Object::RESOLUTION * scrollRate*m_scrollSpeed)*cos(angle);
	pos.y = 300 + 40 * sin(angle) + (count / Object::RESOLUTION * scrollRate*m_scrollSpeed)*sin(angle);

	const double nowCount = m_drawCount - count;
	auto pCount = m_parent->getDrawCount() - nowCount;
	if (m_parent->isFirstTap())
		pCount = 0;
	auto pPos = m_parent->getPos(pCount, scrollRate);

	if (!CanDraw(pPos) && !CanDraw(pos))
		return;



	{
		auto& texture = TextureAsset(L"comet_rainbow_tail");
		{
			const Vec2 pos = m_parent->getPos((3 + (-pCount*scrollRate) / 10000.0)*Pi / 6, count, scrollRate, m_scrollSpeed);
			const Vec2 pPos = m_parent->getPos((3 + (-pCount*scrollRate) / 10000.0)*Pi / 6, pCount, scrollRate);
			Line(pos, pPos).draw(8, ColorF(0, 0.5)).draw(4, { ConvertColor(Palette::Blue),ConvertColor(Palette::Orange) });
			texture.drawAt(pos);
		}
		{
			const Vec2 pos = m_parent->getPos((7 + (-pCount*scrollRate) / 10000.0)*Pi / 6, count, scrollRate, m_scrollSpeed);
			const Vec2 pPos = m_parent->getPos((7 + (-pCount*scrollRate) / 10000.0)*Pi / 6, pCount, scrollRate);
			Line(pos, pPos).draw(8, ColorF(0, 0.5)).draw(4, { ConvertColor(Palette::Red),ConvertColor(Palette::Green) });
			texture.rotate(-4.0*Pi / 3.0).drawAt(pos);
		}
		{
			const Vec2 pos = m_parent->getPos((11 + (-pCount*scrollRate) / 10000.0)*Pi / 6, count, scrollRate, m_scrollSpeed);
			const Vec2 pPos = m_parent->getPos((11 + (-pCount*scrollRate) / 10000.0)*Pi / 6, pCount, scrollRate);
			Line(pos, pPos).draw(8, ColorF(0, 0.5)).draw(4, { ConvertColor(Palette::Yellow),ConvertColor(Palette::Purple) });
			texture.rotate(-2.0*Pi / 3.0).drawAt(pos);
		}

	}


}
