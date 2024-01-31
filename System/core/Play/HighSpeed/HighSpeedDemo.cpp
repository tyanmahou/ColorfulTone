#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <Useful.hpp>
#include <core/Play/PlayStyle/PlayStyle.hpp>
//
#include <core/Object/Note/Note.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <utils/Addon/IntervalCounter.hpp>
#include <Siv3D.hpp>

namespace ct
{
	HighSpeedDemo::HighSpeedDemo() :
		m_offset(-300, 0, Easing::Quad, 400),
		m_bgRect(400 - 45, 0, 90, 500)
	{
		m_style = Game::Config().m_styleType;
		this->resetStyle(m_style);
	}
	HighSpeedDemo::~HighSpeedDemo()
	{
	}

	namespace
	{

		void SpeedUpdate(const s3d::InputGroup& key, double& scrollRate, int32 value)
		{
			if (
				key.down() ||
				key.pressed() && key.pressedDuration() >= 160ms && IntervalCounter::IsUpdatedEvery(5)) {
				int32 tmp = static_cast<int32>(scrollRate * 10);
				tmp += value;
				scrollRate = tmp / 10.0;
			}
		}

	}
	bool HighSpeedDemo::update(double& scrollRate)
	{
		if (auto currentStyle = Game::Config().m_styleType;  m_style != currentStyle) {
			this->resetStyle(currentStyle);
		}
		bool isCtrlPressed = KeyControl.pressed();
		if (KeyControl.down()) {
			if (!m_offset.isMoving()) {
				m_offset.start();
			}
		}
		if (!isCtrlPressed) {
			m_offset.reset();
		}
		if (!m_offset.done())
			return isCtrlPressed;

		if (isCtrlPressed) {
			SpeedUpdate(PlayKey::Up(), scrollRate, 1);
			SpeedUpdate(PlayKey::Down(), scrollRate, -1);
			SpeedUpdate(PlayKey::Right(), scrollRate, 10);
			SpeedUpdate(PlayKey::Left(), scrollRate, -10);

		}
		if (scrollRate < 0.1) {
			scrollRate = 0.1;
		}
		return isCtrlPressed;
	}

	void HighSpeedDemo::resetStyle(PlayStyleType style)
	{
		if (m_style != style) {
			PlayStyle::Instance()->setStyle(style);
		}

		m_style = style;
		m_objects.clear();
		NoteType noteType = style == PlayStyleType::Portrait ? 2 : 5;
		const double barCount = style == PlayStyleType::Default ? 17
			 : style == PlayStyleType::Portrait ? 22
			: 18;
		for (size_t i = 0; i < barCount; ++i) {
			for (size_t j = 0; j < 2; ++j) {
				auto count = NotesData::RESOLUTION * i + NotesData::RESOLUTION * j / 2;
				m_objects.emplace_back(std::make_shared<Note>(0,noteType, static_cast<double>(count), 1));
			}
		}
		for (size_t i = 0; i < barCount; ++i) {
			auto count = NotesData::RESOLUTION * i;
			m_objects.emplace_back(std::make_shared<Bar>(static_cast<double>(count), 1));
		}
	}

	void HighSpeedDemo::drawDemoNotes(const SoundBar& bar, double  scrollRate, size_t index)const
	{
		// マスク処理
		m_bgRect.draw(ColorF(0, 0.5));

		auto scopedMask = Shaders::Mask(index).equal([&] {
			m_bgRect.draw();
		});

		PlayStyle::Instance()->drawJudgeLine();

		const double timePerBar = 60.0 * 4.0 / bar.getBPM();
		const s3d::int64 samplePerBar = static_cast<s3d::int64>(timePerBar * 44100);
		const int64 samples = static_cast<int64>(Math::Fmod(Scene::Time(), timePerBar) * 44100);
		const double f = static_cast<double>(samples % samplePerBar) / samplePerBar;
		const auto nowCount = static_cast<double>(NotesData::RESOLUTION) * f;

		for (const std::shared_ptr<Object>& obj : m_objects | std::ranges::views::reverse) {
			if (obj->getDrawCount() - nowCount >= 0)
				obj->draw(nowCount, scrollRate);
		}
	}
	void HighSpeedDemo::draw(const SoundBar& min, const SoundBar& max, double scrollRate)const
	{
		{
			Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 350,0 }));
			drawDemoNotes(min, scrollRate, 1);
		}
		{
			Transformer2D t2d(Mat3x2::Translate({ m_offset.easeInOut() - 250,0 }));
			drawDemoNotes(max, scrollRate, 2);
		}
	}
}
