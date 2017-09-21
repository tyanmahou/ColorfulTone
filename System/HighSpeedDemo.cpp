#include"HighSpeedDemo.h"
#include"PlayKey.h"
#include"Util.h"
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
	for (int i = 0; i < 10;++i)
	for (int j = 0; j < 2; ++j)
	{
		auto count = NotesData::RESOLUTION*i + NotesData::RESOLUTION*j / 2;
		m_objects.emplace_back(std::make_shared<Note>(5, count, 1));
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

void HighSpeedDemo::update(float& scrollRate)
{
	if (!m_sound.isPlaying())
		m_sound.play();
	
	if (Input::KeyControl.clicked)
	{
		if (!m_offset.isActive())
			m_offset.start();
	}
	if (!Input::KeyControl.pressed)
		m_offset.reset();

	if (!m_offset.isEnd())
		return;

	if (Input::KeyControl.pressed&& +PlayKey::Up().pressed)
	{
		int tmp = scrollRate * 10;
		tmp += 1;
		scrollRate =tmp/10.0f;
		System::Sleep(80);
	}else
	if (Input::KeyControl.pressed&& +PlayKey::Down().pressed)
	{
		int tmp = scrollRate * 10;
		tmp -= 1;
		scrollRate = tmp / 10.0f;
		System::Sleep(80);
	}
	if (scrollRate < 0.1f)
		scrollRate = 0.1f;
}

void HighSpeedDemo::drawDemoNotes(const Mahou::SoundBar& bar, float scrollRate, int stencilNum)const
{
	m_bgRect.draw(ColorF(0, 0.5));

	Graphics2D::SetStencilState(StencilState::Replace);
	Graphics2D::SetStencilValue(stencilNum);

	m_bgRect.draw();

	Graphics2D::SetStencilState(StencilState::Test(StencilFunc::Equal));

	TextureAsset(L"center_base").drawAt(400, 300);
	m_judgeCircle.drawFrame(2, 2, Palette::Orange);

	const auto b = bar(m_sound);

	const auto nowCount = NotesData::RESOLUTION * b.bar + NotesData::RESOLUTION * (b.f);

	for (auto&& elm : m_objects) 
	{
		if(elm->getCount()-nowCount>0)
		elm->draw(nowCount, scrollRate);
	}

	Graphics2D::SetStencilState(StencilState::Default);

}
void HighSpeedDemo::draw(const Mahou::SoundBar& min, const Mahou::SoundBar& max, float scrollRate)const
{
	{
		util::Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 350,0 }));
		drawDemoNotes(min, scrollRate, 3);
	}
	{
		util::Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 250,0 }));
		drawDemoNotes(max, scrollRate, 4);
	}

}