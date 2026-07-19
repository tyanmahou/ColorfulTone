#include <scenes/Scene/EndlessSelect/EndlessSelectScene.hpp>
#include <Useful.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <core/Data/EndlessData/EndlessData.hpp>

#include <Siv3D.hpp>

namespace
{
	using namespace ct;
	EndlessSelectScene::SelectInfo g_selectInfo;

	// エンドレスの絞り込み
	void InitEndless(Array<EndlessData>& endless)
	{
		endless = Game::Endless();

		size_t size = endless.size();
		if (size) {
			g_selectInfo.endless %= size;
		}
		else {
			g_selectInfo.endless = 0;
		}
	}
	// シーン情報のメッセージを取得
	String GetSceneInfoMsg(bool isConfig)
	{
		if (isConfig) {
			return U"[Enter]決定 [BackSpace]戻る";
		}
		// Shift:表示切替
		if (KeyControl.pressed()) {
			return U"[1]プレイモード [2]配置変更 [3]ライフゲージ";
		} else {
			return U"[Ctrl]オプション [Enter]決定";
		}
	}
}

namespace ct
{
	class EndlessSelectScene::Model
	{
	public:
		Model()
		{
			m_config.setActive(false);
		}
		void setData(GameData& data)
		{
			m_data = &data;
		}
		void init()
		{
			::InitEndless(m_endless);
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

			if (!m_stopwatch.isStarted()) {
				m_stopwatch.start();
			}
			// 選択するターゲットの参照
			auto& target = g_selectInfo.endless;
			size_t size = m_endless.size();
			m_moveSelect = SharedLogic::MoveSelectV();
			if (m_moveSelect != 0) {
				if (m_moveSelect < 0) {
					++target;
				} else {
					target += static_cast<uint32>(size);
					--target;
				}
				SoundManager::PlaySe(U"select");
				m_stopwatch.reset();
			}
			target = size ? target % size : 0;

			// 決定ボタン
			if (PlayKey::Start().down() && size) {
				if (m_endless[target].canPlay()) {
					m_isSelectedEndless = true;
				} else {
					System::MessageBoxOK(U"全ての譜面データが存在していないので、このコースはプレイできません。");
				}
			}
			SharedLogic::ChangeLifeGauge();
			SharedLogic::ChangeRandomNoteType();
			SharedLogic::ChangePlayStyle();
		}
		const EndlessData& getSelectEndless() const
		{
			return m_endless[g_selectInfo.endless];
		}
		const Array<EndlessData>& getEndless() const
		{
			return m_endless;
		}

		int32 getMoveSelect()const
		{
			return m_moveSelect;
		}
		bool isSelectedEndless()
		{
			return m_isSelectedEndless;
		}
		const ConfigMain& getConfig()const
		{
			return m_config;
		}
	private:
		GameData* m_data;
		s3d::int32 m_moveSelect = 0;
		Array<EndlessData> m_endless;

		bool m_isSelectedEndless = false;

		Stopwatch m_stopwatch;

		ConfigMain m_config;
	};

	EndlessSelectScene::EndlessSelectScene(const InitData& init) :
		ISceneBase(init),
		m_pModel(std::make_shared<Model>()),
		m_view(this)
	{

		m_pModel->setData(getData());
		m_pModel->init();

		if (!AudioAsset(U"title").isPlaying()) {
			SoundManager::PlayBgm(U"title", 1s);
		}
	}

	void EndlessSelectScene::update()
	{
		m_pModel->update();
		if (m_pModel->isSelectedEndless()) {
			this->changeScene(SceneName::Endless, 1000);
			SoundManager::PlaySe(U"desisionLarge");
		} else if (PlayKey::BigBack().down()) {
			this->changeScene(U"title", 1000);
			SoundManager::PlaySe(U"desisionLarge");
		}
		m_view.update();
	}

	void EndlessSelectScene::finally()
	{
		if (getData().m_toScene == SceneName::Endless) {
			SoundManager::StopBgm(U"title", 1s);
			// データ運搬
			getData().m_endless.init(m_pModel->getSelectEndless(), Game::Config().m_lifeGauge);
			// ライフ引継ぎがあるためスコアを初期化しておく
			getData().m_resultScore = Score(Game::Config().m_lifeGauge);

			// 絶対Autoは解除する
			PlayContext::Revert();
		} else {
			getData().m_course.exit();
		}
	}

	void EndlessSelectScene::draw() const
	{
		m_view.draw();
		// シーン情報
		SceneInfo::DrawEsc();
		SceneInfo::Header(U"[F11] \U000F0493");
		SceneInfo::Draw(GetSceneInfoMsg(m_pModel->getConfig().isActive()));
	}

	void EndlessSelectScene::drawFadeIn(double t) const
	{
		if (getData().m_fromScene == SceneName::Endless) {
			FadeOut(Fade::FlipPage, t, [this]() {draw(); }, false);
		} else {
			FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
		}
	}

	void EndlessSelectScene::drawFadeOut([[maybe_unused]]double t) const
	{
		this->draw();
	}

	EndlessSelectScene::SelectInfo EndlessSelectScene::GetSelectInfo()
	{
		return g_selectInfo;
	}

	const Array<EndlessData>& EndlessSelectScene::getEndless() const
	{
		return m_pModel->getEndless();
	}

	int32 EndlessSelectScene::getMoveSelect() const
	{
		return m_pModel->getMoveSelect();
	}
	const ConfigMain& EndlessSelectScene::getConfig() const
	{
		return m_pModel->getConfig();
	}
}
