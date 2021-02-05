#include"HighSpeedDemo.h"
#include "Useful.hpp"
#include "Constants.hpp"
#include"PlayStyle.h"

#include"Note.h"
#include"Bar.h"

HighSpeedDemo::HighSpeedDemo():
	m_offset(-300,0,Easing::Quad,400), 
	m_bgRect(400 - 45, 0, 90, 500),
	m_judgeCircle(400, 300, 40)
{
//***********************************************************
	//wav取得
	Wave wav;
	//新wavのサンプリング数
	const size_t sample = 44100 * 10 + wav.lengthSample;
	//無音作成
	auto sam = WaveSample(0, 0);
	wav.reserve(sample);
	//10秒かんのwav
	wav.insert(wav.begin(), 44100 * 10, sam);
	m_sound = Sound(wav);
//***********************************************************

	//ノーツ作成
	PlayStyle::Instance()->setStyle(Game::Config().m_styleType);

	for (int i = 0; i < 10;++i)
	for (int j = 0; j < 2; ++j)
	{
		auto count = NotesData::RESOLUTION*i + NotesData::RESOLUTION*j / 2;
		m_objects.emplace_back(std::make_shared<Note>(
			Game::Config().m_styleType==PlayStyleType::Default?	5:2,
			count, 1));
	}
	for (int i = 0; i < 10; ++i)
	{
		m_objects.emplace_back(std::make_shared<Bar>(NotesData::RESOLUTION*i, 1));
	}

}
HighSpeedDemo::~HighSpeedDemo() 
{
	//m_sound.stop();
}

namespace 
{

	void SpeedUpdate(const s3d::KeyCombination key,Stopwatch& sw, float& scrollRate,int value)
	{
		if (
			key.clicked ||
			key.pressed&&sw.ms() >= 80)
		{
			int tmp = scrollRate * 10;
			tmp += value;
			scrollRate = tmp / 10.0f;
			sw.restart();
		}
	}

}
bool HighSpeedDemo::update(float& scrollRate)
{
	bool isCtrlPressed = Input::KeyControl.pressed;
	if (!m_sound.isPlaying())
		m_sound.play();
	
	if (Input::KeyControl.clicked)
	{
		if (!m_offset.isActive())
		{
			m_offset.start();
			m_stopwatch.start();
		}
	}
	if (!isCtrlPressed)
	{
		m_offset.reset();
		m_stopwatch.reset();
	}
	if (!m_offset.isEnd())
		return isCtrlPressed;

	if (isCtrlPressed)
	{
		SpeedUpdate(PlayKey::Up(), m_stopwatch, scrollRate, 1);
		SpeedUpdate(PlayKey::Down(), m_stopwatch, scrollRate, -1);
		SpeedUpdate(PlayKey::Right(), m_stopwatch, scrollRate, 10);
		SpeedUpdate(PlayKey::Left(), m_stopwatch, scrollRate, -10);

	}
	if (scrollRate < 0.1f)
	{
		scrollRate = 0.1f;
	}
	return isCtrlPressed;
}

void HighSpeedDemo::drawDemoNotes(const Mahou::SoundBar& bar, float scrollRate, int stencilNum)const
{
	m_bgRect.draw(ColorF(0, 0.5));

	Graphics2D::SetStencilState(StencilState::Replace);
	Graphics2D::SetStencilValue(stencilNum);

	m_bgRect.draw();

	Graphics2D::SetStencilState(StencilState::Test(StencilFunc::Equal));


	PlayStyle::Instance()->drawJudgeLine();

	const auto b = bar(m_sound);

	const auto nowCount = NotesData::RESOLUTION * b.bar + NotesData::RESOLUTION * (b.f);

	for (auto it = m_objects.rbegin() ;it != m_objects.rend(); ++it)
	{
		if ((*it)->getCount() - nowCount>0)
			(*it)->draw(nowCount, scrollRate);
	}
	Graphics2D::SetStencilState(StencilState::Default);

}
void HighSpeedDemo::draw(const Mahou::SoundBar& min, const Mahou::SoundBar& max, float scrollRate)const
{
	{
		util::Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 350,0 }));
		drawDemoNotes(min, scrollRate, Constants::Stancil::HighSpeedMin);
	}
	{
		util::Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 250,0 }));
		drawDemoNotes(max, scrollRate, Constants::Stancil::HighSpeedMax);
	}

}