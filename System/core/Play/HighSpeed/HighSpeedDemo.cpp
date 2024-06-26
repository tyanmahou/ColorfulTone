﻿#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
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
		m_offset(0, 1, Easing::Quad, 400),
		m_bgRect(400 - 45, 0, 90, 500),
		m_renderTexture{ s3d::RenderTexture(s3d::Scene::Size()), s3d::RenderTexture(s3d::Scene::Size()) }
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
		if (isCtrlPressed) {
			if (!m_offset.done()) {
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
		const NoteType noteType = [style] {
			switch (style) {
			case PlayStyleType::Normal:
			case PlayStyleType::NormalArc:
				return 5;
			case PlayStyleType::Portrait:
			case PlayStyleType::Homography:
			case PlayStyleType::Landscape:
				return 2;
			default:
				return 5;
			}
		}();
		m_bar = std::make_shared<Bar>(0, 0, 1);
		m_note = std::make_shared<Note>(0, noteType, 0, 1);
	}

	void HighSpeedDemo::drawDemoNotes(const s3d::Rect& rect, const SoundBar& bar, double  scrollRate, size_t index)const
	{
		// マスク処理
		rect.draw(ColorF(0, 0.5));

		auto scopedMask = Shaders::Mask(index).equal([&] {
			rect.draw();
		});

		{
			Transformer2D t2d(Mat3x2::Identity(), Transformer2D::Target::SetLocal);

			ScopedRenderTarget2D rt(m_renderTexture[index - 1]);
			m_renderTexture[index - 1].clear(ColorF(0, 0));

			BlendState blend = BlendState::Default2D;
			blend.opAlpha = BlendOp::Max;
			blend.dstAlpha = Blend::DestAlpha;
			blend.srcAlpha = Blend::SrcAlpha;
			ScopedRenderStates2D renderState{ blend };

			s3d::Optional<ScopedCustomShader2D> optVs = m_style == PlayStyleType::Homography ? 
				Shaders::Gizi3D().start() :
				s3d::Optional<ScopedCustomShader2D>(s3d::none);

			Vec2 scaledCenter{ 400,300 };
			if (m_style == PlayStyleType::Homography || m_style == PlayStyleType::Portrait) {
				scaledCenter.y = 500;
			} else  if (m_style == PlayStyleType::Landscape) {
				scaledCenter.x = scaledCenter.y = 200;
			}
			Transformer2D t2dPlayScale(Mat3x2::Scale(Game::Config().m_playScale, scaledCenter));
			PlayStyle::Instance()->drawJudgeLine();

			const double timePerBar = 60.0 * 4.0 / bar.getBPM();
			const s3d::int64 samplePerBar = static_cast<s3d::int64>(timePerBar * 44100);
			const int64 samples = static_cast<int64>(Math::Fmod(Scene::Time(), timePerBar) * 44100);
			const double f = static_cast<double>(samples % samplePerBar) / samplePerBar;
			const auto nowCount = static_cast<double>(NotesData::RESOLUTION) * f;

			int32 barNum = [style = m_style] {
				switch (style) {
				case PlayStyleType::Normal:
					return 18;
				case PlayStyleType::NormalArc:
					return 18;
				case PlayStyleType::Portrait:
					return 22;
				case PlayStyleType::Homography:
					return 50;
				default:
					return 17;
				}
			}();
			barNum = static_cast<int32>(Ceil(barNum / Game::Config().m_playScale));
			for (int32 i = barNum; i >=0; --i) {
				double count = nowCount - static_cast<double>(NotesData::RESOLUTION * i);
				if (count < 0) {
					m_bar->draw(count, scrollRate);
				}
			}
			for (int32 i = barNum; i >= 0; --i) {
				double count = nowCount - static_cast<double>(NotesData::RESOLUTION * i);
				if (count < 0) {
					m_note->draw(count, scrollRate);
				}
			}
		}
		m_renderTexture[index - 1].draw();
	}
	void HighSpeedDemo::draw(const SoundBar& min, const SoundBar& max, double scrollRate)const
	{
		// FIXME 条件分岐つらいからVisitor側に移せると嬉しい
		if (m_style == PlayStyleType::Landscape) {
			Rect bg{ 200 - 100, 200 - 30, 700, 60 };
			{
				Transformer2D t2d(Mat3x2::Translate({ 0, Math::Lerp(200, 0, m_offset.easeInOut()) + 295}));
				drawDemoNotes(bg, min, scrollRate, 1);
			}
			if (min.getBPM() != max.getBPM()) {
				Transformer2D t2d(Mat3x2::Translate({ 0, Math::Lerp(200, 0, m_offset.easeInOut()) + 230}));
				drawDemoNotes(bg, max, scrollRate, 2);
			}
		} else if (m_style == PlayStyleType::Homography) {
			Rect bg{ 400 - 85, 0, 170, 510 };
			{
				Transformer2D t2d(Mat3x2::Translate({ Math::Lerp(-460, 0, m_offset.easeInOut()) - 310,0 }));
				drawDemoNotes(bg, min, scrollRate, 1);
			}
			if (min.getBPM() != max.getBPM()) {
				Transformer2D t2d(Mat3x2::Translate({ Math::Lerp(-460, 0, m_offset.easeInOut()) - 130,0 }));
				drawDemoNotes(bg, max, scrollRate, 2);
			}
		} else {
			{
				Transformer2D t2d(Mat3x2::Translate({ Math::Lerp(-300, 0, m_offset.easeInOut()) - 350,0 }));
				drawDemoNotes(m_bgRect, min, scrollRate, 1);
			}
			if (min.getBPM() != max.getBPM()) {
				Transformer2D t2d(Mat3x2::Translate({ Math::Lerp(-300, 0, m_offset.easeInOut()) - 250,0 }));
				drawDemoNotes(m_bgRect, max, scrollRate, 2);
			}
		}
	}
}
