﻿#include <core/Object/Note/Note.hpp>
#include <Useful.hpp>
#include <core/Judge/Judge.hpp>
#include <core/Input/InputManager.hpp>

#include <core/Object/RepeatNote/RepeatNote.hpp>
#include <core/Play/PlayStyle/PlayStyle.hpp>
#include <core/Play/Random/RandomNote.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <Siv3D.hpp>

namespace {
	//--------------------------------------------------------------------------------
	//関数：GetColor
	//--------------------------------------------------------------------------------
	//概要：ノーツの色を取得
	//--------------------------------------------------------------------------------

	Color GetColor(const ct::NoteType type)
	{
		switch (type) {
		case 1:
		case 11:return Palette::Red;
		case 2:
		case 12:return Palette::Blue;
		case 3:
		case 13:return Palette::Yellow;
		case 4:
		case 14:return Palette::Green;
		case 5:
		case 15:return Palette::Orange;
		case 6:
		case 16:return Palette::Purple;
		}
		return Palette::Black;
	}
}

namespace ct
{
	//--------------------------------------------------------------------------------
	//関数：コンストラクタ
	//--------------------------------------------------------------------------------


	Note::Note(s3d::int64 timingSample, const NoteType type, double firstCount, double speed) :
		Object(timingSample, firstCount),
		m_baseType(type),
		m_type(type),
		m_scrollSpeed(speed)
	{
		m_isClicked[0] = false;
		m_isClicked[1] = false;
		m_isClicked[2] = false;
	};

	//--------------------------------------------------------------------------------
	//関数：init
	//--------------------------------------------------------------------------------
	//概要：初期化
	//--------------------------------------------------------------------------------

	void Note::init()
	{
		Object::init();
		m_isClicked[ColorIndex::Red] = false;
		m_isClicked[ColorIndex::Blue] = false;
		m_isClicked[ColorIndex::Yellow] = false;

		// 配置決定
		m_type = RandomNote::Cast(m_baseType);
		m_color = GetColor(m_type);

		//アクション
		switch (m_type) {
		case 1:
		case 11:
			m_judge = [&]() {
				if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red]) {
					InputManager::PopRed();
					m_isClicked[ColorIndex::Red] = true;
				}
				return m_isClicked[ColorIndex::Red];
				};
			m_clickTimeOffset = [&]()->int64 {
				if (!m_isClicked[ColorIndex::Red]) {
					return InputManager::RedClickedTimeOffset();
				}
				return 0;
				};
			break;
		case 2:
		case 12:
			m_judge = [&]() {
				if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue]) {
					InputManager::PopBlue();
					m_isClicked[ColorIndex::Blue] = true;
				}
				return m_isClicked[ColorIndex::Blue];
				};
			m_clickTimeOffset = [&]()->int64 {
				if (!m_isClicked[ColorIndex::Blue]) {
					return InputManager::BlueClickedTimeOffset();
				}
				return 0;
				};
			break;
		case 3:
		case 13:
			m_judge = [&]() {
				if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow]) {
					InputManager::PopYellow();
					m_isClicked[ColorIndex::Yellow] = true;
				}
				return m_isClicked[ColorIndex::Yellow];
				};
			m_clickTimeOffset = [&]()->int64 {
				if (!m_isClicked[ColorIndex::Yellow]) {
					return InputManager::YellowClickedTimeOffset();
				}
				return 0;
				};
			break;
		case 4:
		case 14:
			m_judge = [&]() {
				if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue]) {
					InputManager::PopBlue();
					m_isClicked[ColorIndex::Blue] = true;
				}
				if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow]) {
					InputManager::PopYellow();
					m_isClicked[ColorIndex::Yellow] = true;
				}
				return m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
				};
			m_clickTimeOffset = [&]()->int64 {
				Optional<int64> max;
				if (!m_isClicked[ColorIndex::Blue]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::BlueClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Yellow]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::YellowClickedTimeOffset());
				}
				return max.value_or(0);
			};
			break;
		case 5:
		case 15:
			m_judge = [&]() {
				if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red]) {
					InputManager::PopRed();
					m_isClicked[ColorIndex::Red] = true;
				}
				if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow]) {
					InputManager::PopYellow();
					m_isClicked[ColorIndex::Yellow] = true;
				}
				return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Yellow];
				};
			m_clickTimeOffset = [&]()->int64 {
				Optional<int64> max;
				if (!m_isClicked[ColorIndex::Red]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::RedClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Yellow]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::YellowClickedTimeOffset());
				}
				return max.value_or(0);
				};
			break;
		case 6:
		case 16:
			m_judge = [&]() {
				if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red]) {
					InputManager::PopRed();
					m_isClicked[ColorIndex::Red] = true;
				}
				if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue]) {
					InputManager::PopBlue();
					m_isClicked[ColorIndex::Blue] = true;
				}
				return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue];
				};
			m_clickTimeOffset = [&]()->int64 {
				Optional<int64> max;
				if (!m_isClicked[ColorIndex::Blue]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::BlueClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Yellow]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::YellowClickedTimeOffset());
				}
				return max.value_or(0);
				};
			break;
		case 7:
		case 17:
			m_judge = [&]() {
				if (InputManager::IsRedClicked() && !m_isClicked[ColorIndex::Red]) {
					InputManager::PopRed();
					m_isClicked[ColorIndex::Red] = true;
				}
				if (InputManager::IsBlueClicked() && !m_isClicked[ColorIndex::Blue]) {
					InputManager::PopBlue();
					m_isClicked[ColorIndex::Blue] = true;
				}
				if (InputManager::IsYellowClicked() && !m_isClicked[ColorIndex::Yellow]) {
					InputManager::PopYellow();
					m_isClicked[ColorIndex::Yellow] = true;
				}
				return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
				};
			m_clickTimeOffset = [&]()->int64 {
				Optional<int64> max;
				if (!m_isClicked[ColorIndex::Red]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::RedClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Blue]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::BlueClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Yellow]) {
					max = Max(max.value_or(std::numeric_limits<int64>::min()), InputManager::YellowClickedTimeOffset());
				}
				return max.value_or(0);
				};
			break;
		case 18:
			m_judge = [&]() {
				if (InputManager::IsRedClicked()) {
					InputManager::PopRed();
					m_isClicked[ColorIndex::Red] = true;
				}
				if (InputManager::IsBlueClicked()) {
					InputManager::PopBlue();
					m_isClicked[ColorIndex::Blue] = true;
				}
				if (InputManager::IsYellowClicked()) {
					InputManager::PopYellow();
					m_isClicked[ColorIndex::Yellow] = true;
				}
				return m_isClicked[ColorIndex::Red] || m_isClicked[ColorIndex::Blue] || m_isClicked[ColorIndex::Yellow];
				};
			m_clickTimeOffset = [&]()->int64 {
				Optional<int64> max;
				if (!m_isClicked[ColorIndex::Red]) {
					max = Min(max.value_or(std::numeric_limits<int64>::max()), InputManager::RedClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Blue]) {
					max = Min(max.value_or(std::numeric_limits<int64>::max()), InputManager::BlueClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Yellow]) {
					max = Min(max.value_or(std::numeric_limits<int64>::max()), InputManager::YellowClickedTimeOffset());
				}
				return max.value_or(0);
				};
			break;
		case 9:
			m_judge = [&]() {
				if (InputManager::IsRedClicked()) {
					m_isClicked[ColorIndex::Red] = true;
				}
				if (InputManager::IsBlueClicked()) {
					m_isClicked[ColorIndex::Blue] = true;
				}
				if (InputManager::IsYellowClicked()) {
					m_isClicked[ColorIndex::Yellow] = true;
				}
				return m_isClicked[ColorIndex::Red] || m_isClicked[ColorIndex::Blue] || m_isClicked[ColorIndex::Yellow];
				};
			m_clickTimeOffset = [&]()->int64 {
				Optional<int64> max;
				if (!m_isClicked[ColorIndex::Red]) {
					max = Min(max.value_or(std::numeric_limits<int64>::max()), InputManager::RedClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Blue]) {
					max = Min(max.value_or(std::numeric_limits<int64>::max()), InputManager::BlueClickedTimeOffset());
				}
				if (!m_isClicked[ColorIndex::Yellow]) {
					max = Min(max.value_or(std::numeric_limits<int64>::max()), InputManager::YellowClickedTimeOffset());
				}
				return max.value_or(0);
				};
			break;
		default:
			m_judge = [] {return false; };
			m_clickTimeOffset = [] {return 0; };
		}
	}

	//--------------------------------------------------------------------------------
	//関数：tapUpdate
	//--------------------------------------------------------------------------------
	//概要：タップ成功時の処理
	//--------------------------------------------------------------------------------
	void Note::tapUpdate(Score::Judge judge, s3d::int64 diff)
	{
		// 白の場合ミス
		if (m_type == 9) {
			judge = Score::Miss;
		}
		//虹色単ノーツは必ずパフェ
		if (m_type == 18) {
			judge = Score::Perfect;
		}

		if (m_type <= 7 || m_type == 9 || m_type == 18) {
			m_isActive = false;
		}

		NoteType type = m_type;
		if (m_type == 18) {
			//　虹色単色の場合押した場所にエフェクトを出す
			type = m_isClicked[ColorIndex::Red] ? 1
				: m_isClicked[ColorIndex::Blue] ? 2
				: 3;
		}
		PlayMusicGame::ScoreUpdate(judge, diff, type, m_type, true);
	}

	//--------------------------------------------------------------------------------
	//関数：tapMiss
	//--------------------------------------------------------------------------------
	//概要：タップミス時の処理
	//--------------------------------------------------------------------------------

	void Note::tapMiss(s3d::int64 diff)
	{
		Score::Judge judge = Score::Miss;
		// 白の場合はパフェ
		if (m_type == 9) {
			judge = Score::Perfect;
		}

		PlayMusicGame::ScoreUpdate(judge, diff, m_type, m_type, false);
		/*
		ロングノーツの場合は、始点が押せなかった時点で終点分も同時にミスとする。
		*/
		if (11 <= m_type && m_type <= 17) {
			PlayMusicGame::ScoreUpdate(judge, diff, m_type, m_type, false);
		}
		m_isActive = false;
	}
	//--------------------------------------------------------------------------------
	//関数：update
	//--------------------------------------------------------------------------------

	bool Note::update(const PlaybackState& state)
	{
		if (!m_isActive)
			return true;

		//ロング用 始点が押されてたらそのままつぎののーつの判定に
		if (isFirstTap()) {
			if (PlayContext::IsAutoPlay())
				InputManager::ForceInput(m_type);
			return true;
		}
		int64 sampleOffset = InputManager::IsHighPrecision() ? m_clickTimeOffset() * 44100 / 1000 : 0;
		const auto timing = m_timingSample - state.samplePos + sampleOffset;

		//ミス
		if (timing < -JudgeRange(Judge::Good) || (m_type == 9 && timing <= 0)) {
			this->tapMiss(timing);
			return true;
		}

		//判定範囲まで到達してなければタップ処理を行わない
		if (timing > JudgeRange(Judge::Good))
			return true;

		//オートプレイ---------------------------------
		if (PlayContext::IsAutoPlay()) {
			if (timing <= OneFrameSample() && m_type != 9) {
				NoteType type = m_type;
				if (type == 18) {
					static int32 tap = 0;
					++tap %= 3;
					type = tap + 1;
					m_isClicked[tap] = true;
				} else {
					m_isClicked[ColorIndex::Red] = true;
					m_isClicked[ColorIndex::Blue] = true;
					m_isClicked[ColorIndex::Yellow] = true;
				}
				this->tapUpdate(Score::Perfect, timing);

				InputManager::ForceInput(type);
				return false;
			}
			return true;
		}


		//---------------------------------------------

		//遅れてしまったか
		if (m_judge()) {
			int64 aTiming = s3d::Abs(timing);
			if (aTiming <= JudgeRange(Judge::Perfect)) {
				this->tapUpdate(Score::Perfect, timing);
			} else if (aTiming <= JudgeRange(Judge::Great)) {
				this->tapUpdate(Score::Great, timing);
			} else if (aTiming <= JudgeRange(Judge::Good)) {
				this->tapUpdate(Score::Good, timing);
			}
			RepeatEnd::notesTapSample = state.samplePos;

			return InputManager::IsAnyClicked();
		}
		return true;
	}

	//----------------------------------------------------------------------------
	//関数：draw
	//----------------------------------------------------------------------------

	void Note::diffDraw(double count, double scrollRate) const
	{
		PlayStyle::Instance()->draw(*this, count, scrollRate);
	}


	//--------------------------------------------------------------------------------
	//関数：isFirstTap
	//--------------------------------------------------------------------------------
	//概要：とりあえず一回はおされたかどうか
	//--------------------------------------------------------------------------------
	bool Note::isFirstTap() const
	{
		switch (m_type) {
		case 1:
		case 11:
			return m_isClicked[ColorIndex::Red];
		case 2:
		case 12:
			return m_isClicked[ColorIndex::Blue];
		case 3:
		case 13:
			return m_isClicked[ColorIndex::Yellow];
		case 4:
		case 14:
			return m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
		case 5:
		case 15:
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Yellow];
		case 6:
		case 16:
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue];
		case 7:
		case 17:
			return m_isClicked[ColorIndex::Red] && m_isClicked[ColorIndex::Blue] && m_isClicked[ColorIndex::Yellow];
		case 18:
			return m_isClicked[ColorIndex::Red] || m_isClicked[ColorIndex::Blue] || m_isClicked[ColorIndex::Yellow];
		}
		return false;
	}
}
