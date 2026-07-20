#include <scenes/Scene/Playlist/PlaylistScene.hpp>
#include <Useful.hpp>

#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <scenes/utils/Audition.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	using MemoInfo = PlaylistScene::MemoInfo;

	MemoInfo g_selectInfo = MemoInfo::Other;
}
namespace ct
{
	class PlaylistScene::Model
	{
	public:
		Model(GameData& data)
		{
			m_config.setActive(false);

			m_data = &data;
			m_title = m_data->session.playlistName();
			m_config.setGameData(m_data);
		}
		const s3d::Optional<s3d::String>& title() const
		{
			return m_title;
		}
		const NotesData& nowNotes()const
		{
			return m_data->session.getNotes();
		}
		void init()
		{
			m_audition.request(nowNotes().getMusic());
		}
		void update()
		{
			if (m_config.isActive()) {
				if (!m_config.update() || KeyF11.down()) {
					m_config.setActive(false);
					m_config.reset();
					SoundManager::PlaySe(U"cancel");
				}
				return;
			} else {
				if (KeyF11.down()) {
					m_config.setActive(true);
					SoundManager::PlaySe(U"desisionSmall");
				}
			}

			if (KeyShift.down()) {
				SoundManager::PlaySe(U"desisionSmall");
				g_selectInfo = g_selectInfo == MemoInfo::Other
					? MemoInfo::Notes : MemoInfo::Other;
			}
			m_highSpeed.update(m_data->m_scrollRate);

			// 1曲目でバックまたはバック長押し
			if ((PlayKey::BigBack().down() || PlayKey::SmallBack().down()) && m_data->session.isFristTrack() ||
				PlayKey::BigBack().pressedDuration() >= 1000ms
				) {
				SoundManager::PlaySe(U"desisionLarge");
				if (m_onBack) {
					m_onBack();
				}
				return;
			}
			if (PlayKey::Start().down()) {
				SoundManager::PlaySe(U"desisionLarge2");
				if (m_onStart) {
					m_onStart();
				}
				return;
			}
			SharedLogic::ChangeRandomNoteType();
			SharedLogic::ChangePlayStyle();
		}
		void postUpdate()
		{
			m_audition.update();
		}
		void finally()
		{
			m_audition.clear();
		}

		const HighSpeedDemo& getHighSpeedDemo()const
		{
			return m_highSpeed;
		}
		const ConfigMain& getConfig()const
		{
			return m_config;
		}

		void subscribeOnStart(std::function<void()>&& callback)
		{
			m_onStart = std::move(callback);
		}
		void subscribeOnBack(std::function<void()>&& callback)
		{
			m_onBack = std::move(callback);
		}

	private:
		GameData* m_data;
		s3d::Optional<s3d::String> m_title;
		Audition m_audition;
		HighSpeedDemo m_highSpeed;
		ConfigMain m_config;

		std::function<void()> m_onStart;
		std::function<void()> m_onBack;
	};
	PlaylistScene::PlaylistScene(const InitData& init) :
		ISceneBase(init),
		m_pModel(std::make_shared<Model>(getData())),
		m_view(this)
	{
		m_pModel->init();
		m_pModel->subscribeOnStart([this] {
			this->changeScene(SceneName::Main, 2000, CrossFade::No);
			});
		m_pModel->subscribeOnBack([this] {
			this->changeScene(SceneName::EndlessSelect, 1000);
			});
	}
	void PlaylistScene::update()
	{
		m_pModel->update();
		m_pModel->postUpdate();

		m_view.update();
	}

	void PlaylistScene::finally()
	{
		m_pModel->finally();
	}

	namespace
	{
		String GetSceneInfo(bool isStart, bool isConfig)
		{
			if (isConfig) {
				return U"[Enter]決定 [BackSpace]戻る";
			}
			if (KeyControl.pressed()) {
				return U"[1]プレイモード [2]配置変更 [Ctrl+↑↓]ハイスピード";
			}
			if (isStart) {
				return U"[Ctrl]オプション [Enter]開始 [BackSpace]戻る";
			}
			return U"[Ctrl]オプション [Enter]開始 [Esc長押し]戻る";
		}
	}
	void PlaylistScene::draw() const
	{
		m_view.draw();

		const bool isStart = getData().session.isFristTrack();
		if (!isStart) {
			SharedDraw::LongPressBack(PlayKey::BigBack(), U"Esc長押しで戻る");
		}
		if (isStart) {
			SceneInfo::DrawEsc();
		}
		SceneInfo::Header(U"[F11] \U000F0493");
		SceneInfo::Draw(::GetSceneInfo(isStart, m_pModel->getConfig().isActive()));
	}

	//--------------------------------------------------------------------------------
	//関数：drawFadeIn
	//--------------------------------------------------------------------------------
	void PlaylistScene::drawFadeIn(double t) const
	{
		FadeIn(Fade::FlipPage, t, [this]() {this->draw(); }, true);
	}

	//--------------------------------------------------------------------------------
	//関数：drawFadeOut
	//--------------------------------------------------------------------------------
	void PlaylistScene::drawFadeOut(double t) const
	{
		if (getData().m_toScene == SceneName::Main) {
			const MusicData music = getData().session.getMusic();
			this->draw();
			FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
			const double size = EaseOut(Easing::Cubic, 300.0, 350.0, t);
			music.getTexture().resized(size, size).drawAt(400, 300, ColorF(1, t * t));
		} else {
			this->draw();
		}
	}
	const s3d::Optional<s3d::String>& PlaylistScene::title() const
	{
		return m_pModel->title();
	}
	const PlaySession& PlaylistScene::getPlay() const
	{
		return getData().session;
	}

	const HighSpeedDemo& PlaylistScene::getHighSpeedDemo() const
	{
		return m_pModel->getHighSpeedDemo();
	}

	const ConfigMain& PlaylistScene::getConfig() const
	{
		return m_pModel->getConfig();
	}

	MemoInfo PlaylistScene::GetMemoInfo()
	{
		return g_selectInfo;
	}
}
