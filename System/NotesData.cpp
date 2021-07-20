#include"NotesData.h"
#include"GenreManager.h"
#include<queue>
#include"Note.h"
#include"Bar.h"
#include"TextObject.h"
#include"LongNote.h"
#include"RepeatNote.h"
#include"ScoreLoader.hpp"

namespace
{
	unsigned GetLastStopIndex(const double count, const Array<StopInfo>& stopInfos)
	{
		for (unsigned k = 0; k < stopInfos.size(); ++k)
		{
			if (count < stopInfos[k].m_firstCount)
			{
				return k - 1;
			}
		}
		return stopInfos.size() - 1;
	}

	double GetJudgeOffset(const double count, const Array<StopInfo>& stopInfos)
	{
		double judgeOffset = 0;

		unsigned lastStopIndex = GetLastStopIndex(count, stopInfos);

		if (count > stopInfos[lastStopIndex].m_firstCount)
			judgeOffset = stopInfos[lastStopIndex].m_totalOffset;
		else
			judgeOffset = stopInfos[lastStopIndex - 1].m_totalOffset;

		return judgeOffset;
	}
}
NotesData::NotesData(
	const MusicData*const pMusic,
	const String& dirPath,
	const String& filePath,
	uint32 index
) : m_bpm(120),
m_offsetSample(0),
m_lv(0),
m_lvName(L"None"),
m_notesArtistName(L"None"),
m_totalNotes(0),
m_maxBarCount(0),
m_fileName(filePath.remove(L".csv")),
m_color(Palette::White),
m_pMusic(pMusic),
m_index(index)
{
	CSVReader csv(dirPath + filePath);
	if (!csv)
		return;
	//セーブデータの読み込み
	m_score = ScoreLoader::Load(this->getScorePath());

	//ここから譜面データ
	this->load(csv);
}
void NotesData::init()
{
	RepeatEnd::notesTapCount = 0;
	for (auto&& elm : m_objects)
	{
		elm->init();
	}
}

void NotesData::synchroCount(const Sound & sound, double & nowCount)
{
	m_currentBarIndex = 0;
#ifdef VIEWER_MODE
	const auto sample = sound.streamPosSample();
#else
	const auto sample = sound.samplesPlayed();
#endif

	for (unsigned int i = 0; i < m_tempoInfos.size(); ++i)
	{
		if (sample >= m_tempoInfos.at(i).m_changeSample)
		{
			m_currentBarIndex = i;
		}
	}

	auto& currentTempo = m_tempoInfos.at(m_currentBarIndex).m_bar;
	const auto b = currentTempo(sound);

	nowCount = NotesData::RESOLUTION * b.bar + NotesData::RESOLUTION * (b.f);
}

void NotesData::update(Sound& sound, double& nowCount, Score& score)
{

	this->synchroCount(sound, nowCount);

	if (sound.streamPosSec() < 3)
		return;

	/*
	bpm= beat / 1 min
	bpm/60 = beat / 1sec
	bpm/60 : x = 4 : NotesData::RESOLUTION
	x=bpm*NotesData::RESOLUTION/240

	count / fream = x/60;
	*/
	auto& currentTempo = m_tempoInfos.at(m_currentBarIndex).m_bar;

	double countPerFrame = currentTempo.getBPM()*NotesData::RESOLUTION / 14400;

	for (auto&& elm : m_objects)
	{
		if (!elm->update(nowCount, countPerFrame))
		{
			break;
		}
	}
}
void NotesData::draw(const double& nowCount, float scrollRate)const
{
	for (auto it = m_objects.rbegin(); it != m_objects.rend(); it++)
	{
		(*it)->draw(nowCount, scrollRate);
	}
}

void NotesData::previewDraw(const double& nowCount, float scrollRate)const
{
	for (auto it = m_objects.rbegin(); it != m_objects.rend(); it++)
	{
		if ((*it)->getDrawCount() >= nowCount)
			(*it)->draw(nowCount, scrollRate);
	}
}

String NotesData::getLevelWithStar() const
{
	if (m_starLv != StarLv::None) {
		return ToStr(m_starLv) + L":" + Format(m_lv);
	}
	return Format(m_lv);
}

const String NotesData::getLevelNameAndLevel() const
{
	return m_lvName + L" Lv" + this->getLevelWithStar();
}

String NotesData::getScorePath()const
{
	return L"UserData/Score/"
		+ m_pMusic->getGenreName() + L"/"
		+ m_pMusic->getFileName() + L"/"
		+ this->m_fileName
		+ L".bin";
}

void NotesData::load(CSVReader & csv)
{
	auto rows = csv.rows;				//行数
	String head;						//1列目のデータを文字列で
	int bar = 0;						//現在入力中の小節
	std::queue<double> noteSpeed;		//ノーツのスピード変化を覚えておく
	std::queue<double> barSpeed;		//小節線のスピード変化を覚えておく
	std::queue<double> measures;		//拍子記憶用
	double nowMeasure = 1.0;				//拍子の初期化
	std::shared_ptr<Note> parentNote = std::make_shared<Note>(0, 0, 0);	//ノーツの記憶(ロング用)
	int totalNotes = 0;					//ノーツ数
	double scrollBaseSpeed = 1.0;
	double repeatInterval = 8.0;					//連打間隔

	BPMType nowBPM = 120;

	//譜面停止用
	Array<StopInfo> stopInfos;
	stopInfos.emplace_back();
	stopInfos.emplace_back();
	//bpm変更用
	Array<TempoInfo> tempoInfo;
	double lastBPMChangeCount = 0;
	int64 totalSample = 44100 * 4;

	double nowCount = 0;

	for (unsigned int i = 0; i < rows; i++)
	{
		head = csv.get<String>(i, 0);

		if (head.isEmpty)			//空行はスルー
		{
			continue;
		}
		if (head[0] == '%')//コメント
		{
			continue;
		}
		if (head[0] != '#')			//ノーツ
		{
			int col = csv.columns(i);

			//小節線のスピード保存
			double bs = noteSpeed.empty() ? scrollBaseSpeed : noteSpeed.front();
			barSpeed.push(bs);

			for (int j = 0; j < col; ++j) //この小節のノーツ読み込み
			{
				int type = csv.get<int>(i, j);

				if (!type) {
					if (!noteSpeed.empty())
						noteSpeed.pop();
					continue;	//0の場合スルー
				}

				//ノーツ数数える
				if (type != 10)
					totalNotes++;

				//カウント値
				double count = nowCount + NotesData::RESOLUTION*nowMeasure*j / col;

				double judgeOffset = GetJudgeOffset(count, stopInfos);

				//ノーツのスピード
				double spd;
				if (!noteSpeed.empty()) {
					spd = noteSpeed.front();
					noteSpeed.pop();
				}
				else
					spd = scrollBaseSpeed;

				//ノーツ生成
				std::shared_ptr<Note> note;

				if (type == 10)//連打ノーツかどうか
				{
					note = std::make_shared<RepeatNote>(count + judgeOffset, spd);
				}
				else
				{
					note = std::make_shared<Note>(type, count + judgeOffset, spd);
				}
				if (type >= 10 && type <= 17)//ロングノーツの場合親ノーツを保存
					parentNote = note;

				//ベクタに追加
				if (type == 8)
				{
					if (parentNote->getType() == 10)//親が連打ノーツか
					{
						m_objects.emplace_back(std::make_shared<RepeatEnd>(count + judgeOffset, spd, parentNote, repeatInterval));
					}
					else
					{
						m_objects.emplace_back(std::make_shared<LongNote>(parentNote->getType(), count + judgeOffset, spd, parentNote));
					}
				}
				else {
					m_objects.emplace_back(note);
				}

			}
			measures.push(nowMeasure);
			nowCount += NotesData::RESOLUTION*nowMeasure;
			bar++;					//次の小節へ
		}
		else						//その他
		{
			if (head == L"#NOTE")
			{
				m_notesArtistName = csv.getOr<String>(i, 1, L"None");
			}
			else if (head == L"#LEVEL")
			{
				m_lv = csv.getOr<int>(i, 1, 0);
				m_lvName = csv.getOr<String>(i, 2, L"None");

				auto starKind = csv.getOpt<String>(i, 3);
				if (starKind) {
					if (*starKind == L"★") {
						m_starLv = StarLv::One;
					} else if (*starKind == L"★★") {
						m_starLv = StarLv::Two;
					}
				} else {
					if (m_lv == 14) {
						m_starLv = StarLv::One;
					} else if (m_lv >= 15) {
						m_starLv = StarLv::Two;
					}
				}
			}
			else if (head == L"#BPM")
			{
				m_bpm = csv.getOr<BPMType>(i, 1, 120);
				nowBPM = m_bpm;
			}
			else if (head == L"#OFFSET")
			{
				m_offsetSample = csv.getOr<int>(i, 1, 0);
				totalSample += m_offsetSample;
			}
			else if (head == L"#SCROLL")
			{
				int col = csv.columns(i);
				for (int j = 1; j < col; ++j)
					noteSpeed.push(csv.get<double>(i, j));
			}
			else if (head == L"#BPMCHANGE")
			{
				/*
				240:9600=x:n
				240*n/9600
				*/
				double count = nowCount + NotesData::RESOLUTION*nowMeasure*csv.getOr<double>(i, 2, 0) / csv.getOr<double>(i, 3, 1);
				const double judgeOffset = GetJudgeOffset(count, stopInfos);
				count += judgeOffset;

				const BPMType bpm = csv.getOr<BPMType>(i, 1, 120);
				{
					const double tmp1 = 4 * 44100 * 60 / nowBPM;
					const double tmp2 = 4 * 44100 * 60 / bpm;

					double preBPMSample = (count - lastBPMChangeCount)*tmp1 / static_cast<double>(NotesData::RESOLUTION);
					totalSample += preBPMSample;
					tempoInfo.emplace_back(totalSample, totalSample - count / static_cast<double>(NotesData::RESOLUTION)*tmp2, bpm);
				}
				nowBPM = bpm;
				lastBPMChangeCount = count;
			}
			else if (head == L"#STOP")
			{
				double count = nowCount + NotesData::RESOLUTION*nowMeasure*csv.getOr<double>(i, 3, 0) / csv.getOr<double>(i, 4, 1);
				double judgeOffset = GetJudgeOffset(count, stopInfos);
				double range = NotesData::RESOLUTION*nowMeasure*csv.getOr<double>(i, 1, 0) / csv.getOr<double>(i, 2, 1);
				m_stopRanges.emplace_back(count + judgeOffset, range);
			}
			else if (head == L"#DIRECTSTOP")
			{
				double count = nowCount + NotesData::RESOLUTION*nowMeasure*csv.getOr<double>(i, 2, 0) / csv.getOr<double>(i, 3, 1);
				const double stopSec = csv.getOr<double>(i, 1, 120);
				const double range = stopSec * nowBPM*static_cast<double>(NotesData::RESOLUTION) / 240.0;
				stopInfos.emplace_back(count, stopInfos[stopInfos.size() - 1].m_totalOffset + range);

				double judgeOffset = GetJudgeOffset(count, stopInfos);
				m_stopRanges.emplace_back(count + judgeOffset, range);
			}
			else if (head == L"#MEASURE")
			{
				nowMeasure = csv.getOr<double>(i, 1, 1.0) / csv.getOr<double>(i, 2, 1.0);
			}
			else if (head == L"#SCROLLBASE")
			{
				scrollBaseSpeed = csv.getOr<double>(i, 1, 1.0);
			}
			else if (head == L"#INTERVAL")
			{
				repeatInterval = csv.getOr<double>(i, 1, 8.0);
				if (repeatInterval <= 0)
				{
					repeatInterval = 8.0;
				}
			}
			else if (head == L"#TEXT")
			{
				double count = nowCount + NotesData::RESOLUTION*nowMeasure*csv.getOr<double>(i, 3, 0) / csv.getOr<double>(i, 4, 1);
				const double drawSec = csv.getOr<double>(i, 2, 2);
				const String msg = csv.getOr<String>(i, 1, L"");

				double judgeOffset = GetJudgeOffset(count, stopInfos);

				m_objects.emplace_back(std::make_shared<TextObject>(count + judgeOffset, msg, drawSec));
			}
			else if (head == L"#COLOR")
			{
				const String colorHex = csv.getOr<String>(i, 1, L"#FFFFFF");
				m_color = Color(colorHex);
			}
			else if (head == L"#END")
			{
				break;
			}
		}
	}

	m_totalNotes = totalNotes;

	//小節線作成

	double nowBarCount = 0;

	for (int i = 0; i < bar; ++i)
	{
		double judgeOffset = GetJudgeOffset(nowBarCount, stopInfos);
		//		nowBarCount += judgeOffset;
		m_objects.emplace_back(std::make_shared<Bar>(nowBarCount + judgeOffset, barSpeed.front()));
		nowBarCount += NotesData::RESOLUTION*measures.front();
		measures.pop();
		barSpeed.pop();
	}

	for (auto&& note : m_objects)
	{
		for (auto&& s : m_stopRanges)
		{
			note->addStopCount(s);
		}
	}

	m_maxBarCount = nowBarCount + GetJudgeOffset(nowBarCount, stopInfos) + NotesData::RESOLUTION * 2;

	m_tempoInfos.emplace_back(0, 44100 * 4 + m_offsetSample, m_bpm);
	std::copy(tempoInfo.begin(), tempoInfo.end(), std::back_inserter(m_tempoInfos));

	//同期クラス作成等の初期化
	init();

	int lv = m_lv;
	auto starLv = m_starLv;
	if (m_starLv != StarLv::None) {
		GenreManager::Add(GenreType::StarLv, Format(L"LEVEL:", ToStr(starLv)), [starLv](MusicData& music)->bool {
			return s3d::AnyOf(music.getNotesData(), [starLv](const NotesData& notes) {return notes.getStarLv() == starLv; });
		}, static_cast<int>(starLv));
	} else {
		GenreManager::Add(GenreType::Lv, Format(L"LEVEL:", lv), [lv](MusicData& music)->bool {
			return s3d::AnyOf(music.getNotesData(), [lv](const NotesData& notes) {return notes.getLevel() == lv; });
		}, lv);
	}
}

void NotesData::saveScore(const ScoreModel & score) const
{
	ScoreLoader::Save(this->getScorePath(), score);
}
