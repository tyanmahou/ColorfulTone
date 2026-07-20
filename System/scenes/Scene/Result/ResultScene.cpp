#include <scenes/Scene/Result/ResultScene.hpp>
#include <core/Play/Score/PlayingScore.hpp>
#include <core/Play/Score/CourseResult.hpp>
#include <core/Play/Score/EndlessResult.hpp>
#include <core/Play/Session/Course/PlayCourse.hpp>
#include <core/Play/Session/Endless/PlayEndless.hpp>
#include <core/Data/EndlessData/EndlessData.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace
{	
	using namespace ct;

	// スコアの更新　newRecordのばあいtrue
	bool UpdateScore(const ScoreModel& score, const NotesData& notes)
	{
		ScoreModel srcScore = notes.getScore();
		bool isNewRecord = false;
		bool isUpdate = false;
		if (score.isClear && !srcScore.isClear)
		{
			srcScore.isClear = true;
			isUpdate = true;
		}
		if (score.specialResult > srcScore.specialResult)
		{
			srcScore.specialResult = score.specialResult;
			isUpdate = true;
		}
		if (score.clearRate > srcScore.clearRate)
		{
			srcScore.clearRate = score.clearRate;
			isNewRecord = true;
			isUpdate = true;
		}
		if (score.isLifeClear && !srcScore.isLifeClear) {
			srcScore.isLifeClear = true;
			isUpdate = true;
		}
		if (score.isLifeClear && score.gauge > srcScore.gauge) {
			srcScore.gauge = score.gauge;
			isUpdate = true;
		}
		if (isUpdate)
		{
			notes.saveScore(srcScore);
		}
		return isNewRecord;
	}
	// コーススコアの更新　newRecordのばあいtrue
	bool UpdateCourseScore(const CourseScore& score, const CourseData& course)
	{
		CourseScore srcScore = course.getScore();
		bool isNewRecord = false;
		bool isFirstClear = false;
		if (score.isClear && !srcScore.isClear)
		{
			srcScore.isClear = true;
			isNewRecord = true;
			isFirstClear = true;
		}
		if (static_cast<uint8>(score.special) > static_cast<uint8>(srcScore.special))
		{
			srcScore.special = score.special;
			isNewRecord = true;
		}
		if (score.totalRate > srcScore.totalRate)
		{
			srcScore.totalRate = score.totalRate;
			isNewRecord = true;

		}
		if (score.isLifeClear && !srcScore.isLifeClear) {
			srcScore.isLifeClear = true;
			isNewRecord = true;
		}

		bool isLifeUpdate = score.life > srcScore.life && (score.gauge == srcScore.gauge || srcScore.gauge == LifeGaugeKind::None);
		bool isGaugeUpdate = score.isLifeClear && score.gauge > srcScore.gauge;
		if (isFirstClear || isLifeUpdate || isGaugeUpdate)
		{
			srcScore.gauge = score.gauge;
			srcScore.life = score.life;
			isNewRecord = true;
		}
		if (isNewRecord)
		{
			course.saveScore(srcScore);
		}
		return isNewRecord;
	}
	bool UpdateEndlessScore(const EndlessResult& result, const EndlessData& endless)
	{

		const EndlessGaugeScore& newResult = result.score;
		EndlessGaugeScore srcScore = endless.getScore(result.gauge);

		bool isNewRecord = false;
		if (newResult.clearCount > srcScore.clearCount) {
			srcScore.clearCount = newResult.clearCount;
			isNewRecord = true;
		}
		if (newResult.maxCombo > srcScore.maxCombo) {
			srcScore.maxCombo = newResult.maxCombo;
			isNewRecord = true;
		}
		if (isNewRecord) {
			endless.saveScore(result.gauge, srcScore);
		}
		return isNewRecord;
	}
	detail::FormatHelper GetCourseTweetText(const s3d::Optional<CourseResult>& courseResult)
	{
		String playingText;
		if (courseResult && !courseResult->isEnd) {
			auto kind = courseResult->passKind();
			if (kind == CoursePassKind::Pass) {
				playingText = U"{}に合格!!!";
			} else if (kind == CoursePassKind::KariPass) {
				playingText = U"{}に仮合格!!!";
			} else {
				playingText = U"{}に不合格…";
			}
		} else {
			playingText = U"{}をプレイ中";
		}

		return s3d::Fmt(playingText + U"\n{:.2f}%達成\n#ColorfulTone");
	}
}
namespace ct
{
	class ResultScene::Model
	{
	public:

		Model(GameData& data)
		{
			m_data = &data;

			// sessionは破棄されるかもしれないので、コピーする
			m_notes = m_data->session.getNotes();
			m_score = m_data->session.getScore();
			m_result = m_data->session.getResult();
			if (const auto* pCoureResult = m_data->session.getCourseResult()) {
				m_courseResult = *pCoureResult;
			}
			if (const auto* pEndlessResult = m_data->session.getEndlessResult()) {
				m_endlessResult = *pEndlessResult;
			}
			m_playlistName = m_data->session.playlistName();
			saveScore();
		}
		const s3d::Optional<s3d::String>& playlistName() const
		{
			return m_playlistName;
		}
		void saveScore()
		{
			// autoのばあいセーブしない
			if (PlayContext::CanNotUpdateScore()) {
				return;
			}
			m_isNewRecord = ::UpdateScore(m_result, m_notes);
			if (m_data->session.isEnd()) {
				// コース
				if (auto course = m_data->session.cast<PlayCourse>()) {
					::UpdateCourseScore(m_courseResult->score, course->getCourse());
				}
				// エンドレス
				else if (auto endless = m_data->session.cast<PlayEndless>()) {
					::UpdateEndlessScore(*m_endlessResult, endless->getEndless());
				}
			}
		}
		bool isNewRecord()const
		{
			return m_isNewRecord;
		}

		String getTweetText()const
		{
			if (m_courseResult) {
				return GetCourseTweetText(m_courseResult)(*m_playlistName, m_courseResult->score.totalRate);
			}
			const MusicData music = m_notes.getMusic();
			if (m_endlessResult) {
				s3d::StringView fmt;
				if (m_endlessResult->isEnd) {
					fmt = U"{}/{}で{:.2f}%達成\n{}で、{}曲連続クリア、{}コンボ達成\n#ColorfulTone";
				}
				else {
					fmt = U"{}/{}で{:.2f}%達成\n{}で、{}曲連続プレイ中、{}コンボ達成\n#ColorfulTone";
				}
				return s3d::Fmt(fmt)(
					music.getMusicName(),
					m_notes.getLevelName(),
					m_result.clearRate,
					*m_playlistName,
					m_endlessResult->score.clearCount,
					m_endlessResult->score.maxCombo
				);
			}
			return U"{}/{}で{:.2f}%達成\n#ColorfulTone"_fmt(
				music.getMusicName(),
				m_notes.getLevelName(),
				m_result.clearRate
			);
		}
		const NotesData& getNotes() const
		{
			return m_notes;
		}

		const PlayingScore& getScore()const
		{
			return m_score;
		}
		const ScoreModel& getResult() const
		{
			return m_result;
		}
		const s3d::Optional<CourseResult>& getCourseResult() const
		{
			return m_courseResult;
		}
		const s3d::Optional<EndlessResult>& getEndlessResult() const
		{
			return m_endlessResult;
		}
	private:
		GameData* m_data;
		s3d::Optional<s3d::String> m_playlistName;

		NotesData m_notes;
		PlayingScore m_score;
		ScoreModel m_result;
		s3d::Optional<CourseResult> m_courseResult;
		s3d::Optional<EndlessResult> m_endlessResult;

		bool m_isNewRecord = false;
	};

	ResultScene::ResultScene(const InitData& init) :
		ISceneBase(init),
		m_model(std::make_shared<Model>(getData())),
		m_view(this)
	{
		SoundManager::PlayBgm(U"result", 1s);
		m_view.init();
	}
	void ResultScene::finally()
	{
		SoundManager::StopBgm(U"result", 1s);
		if (!getData().session.isEnd()) {
			getData().session.next();
		}
	}

	void ResultScene::update()
	{
		if (KeyT.down()) {
			// ツイート
			Twitter::OpenTweetWindow(m_model->getTweetText());
		}
		if (PlayKey::Start().down() || PlayKey::BigBack().down()) {
			SoundManager::PlaySe(U"desisionLarge");
			if (getData().session.isEnd()) {
				this->changeScene(getData().session.selectScene(), 1000);
				getData().session.exit();
			}
			else {
				this->changeScene(getData().session.playlistScene(), 1000);
			}
		}
		if (KeyF10.down()) {
			const MusicData selectMusic = getData().session.getMusic();
			bool isFavorite = !selectMusic.isFavorite();
			size_t index = selectMusic.getIndex();
			MusicData& sourceMusic = Game::Musics()[index];
			sourceMusic.saveFavorite(isFavorite);
			SoundManager::PlaySe(U"desisionSmall");
		}
		m_view.update();
	}

	void ResultScene::draw() const
	{
		m_view.draw();
		SceneInfo::DrawEsc();
		SceneInfo::Header(U"[T] \U000F0544 [F10] \U000F1563");
		SceneInfo::Draw(U"[Enter]戻る");
	}

	void ResultScene::drawFadeIn(double t) const
	{
		this->draw();
		FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
	}

	void ResultScene::drawFadeOut([[maybe_unused]]double t) const
	{
		this->draw();
	}

	const NotesData& ResultScene::getNotes() const
	{
		return m_model->getNotes();
	}

	const ScoreModel& ResultScene::getResult() const
	{
		return m_model->getResult();
	}

	const PlayingScore& ResultScene::getScore() const
	{
		return m_model->getScore();
	}

	const s3d::Optional<CourseResult>& ResultScene::getCourseResult() const
	{
		return m_model->getCourseResult();
	}
	const s3d::Optional<EndlessResult>& ResultScene::getEndlessResult() const
	{
		return m_model->getEndlessResult();
	}
	bool ResultScene::isNewRecord() const
	{
		return m_model->isNewRecord();
	}
	const s3d::Optional<s3d::String>& ResultScene::playlistName() const
	{
		return m_model->playlistName();
	}
}