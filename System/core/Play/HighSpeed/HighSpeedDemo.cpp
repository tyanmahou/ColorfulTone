#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <Useful.hpp>
//#include "Constants.hpp"
#include <core/Play/PlayStyle/PlayStyle.hpp>
//
#include <core/Object/Note/Note.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <Siv3D.hpp>

namespace ct
{
	HighSpeedDemo::HighSpeedDemo() :
		m_offset(-300, 0, Easing::Quad, 400),
		m_bgRect(400 - 45, 0, 90, 500),
		m_judgeCircle(400, 300, 40)
	{
		//***********************************************************
			//wav取得
		Wave wav;
		//新wavのサンプリング数
		const size_t sample = 44100 * 10 + wav.lengthSample();
		//無音作成
		auto sam = WaveSample(0, 0);
		wav.reserve(sample);
		//10秒かんのwav
		wav.insert(wav.begin(), 44100 * 10, sam);
		m_sound = Audio(wav);
		//***********************************************************
		//ノーツ作成
		PlayStyle::Instance()->setStyle(Game::Config().m_styleType);

		for (size_t i = 0; i < 10; ++i)
			for (size_t j = 0; j < 2; ++j) {
				auto count = NotesData::RESOLUTION * i + NotesData::RESOLUTION * j / 2;
				m_objects.emplace_back(std::make_shared<Note>(
					Game::Config().m_styleType == PlayStyleType::Default ? 5 : 2,
					static_cast<double>(count), 1));
			}
		for (size_t i = 0; i < 10; ++i) {
			m_objects.emplace_back(std::make_shared<Bar>(static_cast<double>(NotesData::RESOLUTION * i), 1));
		}

	}
	HighSpeedDemo::~HighSpeedDemo()
	{
		//m_sound.stop();
	}

	namespace
	{

		void SpeedUpdate(const s3d::InputGroup& key, Stopwatch& sw, double& scrollRate, int32 value)
		{
			if (
				key.down() ||
				key.pressed() && sw.ms() >= 80) {
				int32 tmp = static_cast<int32>(scrollRate * 10);
				tmp += value;
				scrollRate = tmp / 10.0;
				sw.restart();
			}
		}

	}
	bool HighSpeedDemo::update(double& scrollRate)
	{
		bool isCtrlPressed = KeyControl.pressed();
		if (!m_sound.isPlaying())
			m_sound.play();

		if (KeyControl.down()) {
			if (!m_offset.isMoving()) {
				m_offset.start();
				m_stopwatch.start();
			}
		}
		if (!isCtrlPressed) {
			m_offset.reset();
			m_stopwatch.reset();
		}
		if (!m_offset.done())
			return isCtrlPressed;

		if (isCtrlPressed) {
			SpeedUpdate(PlayKey::Up(), m_stopwatch, scrollRate, 1);
			SpeedUpdate(PlayKey::Down(), m_stopwatch, scrollRate, -1);
			SpeedUpdate(PlayKey::Right(), m_stopwatch, scrollRate, 10);
			SpeedUpdate(PlayKey::Left(), m_stopwatch, scrollRate, -10);

		}
		if (scrollRate < 0.1) {
			scrollRate = 0.1;
		}
		return isCtrlPressed;
	}

	void HighSpeedDemo::drawDemoNotes(const SoundBar& bar, double  scrollRate)const
	{
		// TODO マスク処理
		//m_bgRect.draw(ColorF(0, 0.5));

		//Graphics2D::SetStencilState(StencilState::Replace);
		//Graphics2D::SetStencilValue(stencilNum);

		//m_bgRect.draw();

		//Graphics2D::SetStencilState(StencilState::Test(StencilFunc::Equal));


		//PlayStyle::Instance()->drawJudgeLine();

		//const auto b = bar(m_sound);

		//const auto nowCount = NotesData::RESOLUTION * b.bar + NotesData::RESOLUTION * (b.f);

		//for (auto it = m_objects.rbegin(); it != m_objects.rend(); ++it) {
		//	if ((*it)->getCount() - nowCount > 0)
		//		(*it)->draw(nowCount, scrollRate);
		//}
		//Graphics2D::SetStencilState(StencilState::Default);

	}
	void HighSpeedDemo::draw(const SoundBar& min, const SoundBar& max, double scrollRate)const
	{
		{
			Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 350,0 }));
			drawDemoNotes(min, scrollRate);
		}
		{
			Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 250,0 }));
			drawDemoNotes(max, scrollRate);
		}

	}
}
