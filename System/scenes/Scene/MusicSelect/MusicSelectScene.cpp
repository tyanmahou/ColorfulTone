#include <scenes/Scene/MusicSelect/MusicSelectScene.hpp>
#include <Useful.hpp>
#include <core/Input/AutoPlayManager.hpp>
#include <core/HighSpeed/HighSpeedDemo.hpp>
#include <scenes/utils/Audition.hpp>
#include <scenes/utils/SharedLogic.hpp>
#include <core/Data/Genre/GenreManager.hpp>
#include <scenes/Scene/KeyConfig/ConfigMain.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;

    MusicSelectScene::SelectMusicsInfo g_selectInfo;

    using SortMode = MusicSelectScene::SortMode;
    using Action = MusicSelectScene::Action;
    //　ソートの切り替え用
    SortMode NextMode(SortMode mode)
    {
        switch (mode) {
        case SortMode::FileName:return SortMode::MusicName;
        case SortMode::MusicName:return SortMode::ArtistName;
        case SortMode::ArtistName:return SortMode::LastUpdateAt;
        case SortMode::LastUpdateAt:return SortMode::FileName;
        }
        return SortMode::Default;
    }
    // ソート関数の取得
    std::function<bool(const MusicData& l, const MusicData& r)> SortFunc(SortMode mode)
    {
        switch (mode) {
        case SortMode::FileName:return [](const MusicData& l, const MusicData& r) {
            return 	l.getIndex() < r.getIndex();
            };
        case SortMode::MusicName:return  [](const MusicData& l, const MusicData& r) {
            if (l.getMusicName() == r.getMusicName()) {
                return l.getIndex() < r.getIndex();
            }
            return 	l.getMusicName() < r.getMusicName();
            };
        case SortMode::ArtistName:return  [](const MusicData& l, const MusicData& r) {
            if (l.getArtistName() == r.getArtistName()) {
                return l.getIndex() < r.getIndex();
            }
            return 	l.getArtistName() < r.getArtistName();
            };
        case SortMode::LastUpdateAt:return  [](const MusicData& l, const MusicData& r) {
            if (l.getLastUpdateAt() == r.getLastUpdateAt()) {
                return l.getIndex() < r.getIndex();
            }
            return 	l.getLastUpdateAt() > r.getLastUpdateAt();
            };
        }
        return SortFunc(SortMode::Default);
    }

    // 楽曲リストソート
    void RefineMusics(Array<MusicData>& musics)
    {
        musics.remove_if([&](const MusicData& m) {
            return !GenreManager::GetRefiner(g_selectInfo.genre)(m);
        });
    }

    // 楽曲リストソート
    void SortMusics(Array<MusicData>& musics)
    {
        musics.stable_sort_by(SortFunc(g_selectInfo.sortMode));
    }

    // 楽曲リスト初期化
    void InitMusics(Array<MusicData>& musics)
    {
        musics = Game::Musics();
        ::RefineMusics(musics);
        ::SortMusics(musics);

        size_t musicSize = musics.size();
        if (musicSize) {
            g_selectInfo.music %= musicSize;
        } else {
            g_selectInfo.music = 0;
        }
    }

    uint32& GetSelectTarget(Action action)
    {
        switch (action) {
        case Action::GenreSelect: return g_selectInfo.genre;
        case Action::MusicSelect: return g_selectInfo.music;
        case Action::LevelSelect: return g_selectInfo.level;
        default:
            break;
        }
        return g_selectInfo.music;
    }
    size_t GetTargetSize(Action action, const Array<MusicData>& musics)
    {
        switch (action) {
        case Action::GenreSelect: return GenreManager::Size();
        case Action::MusicSelect: return musics.size();
        case Action::LevelSelect: return musics[g_selectInfo.music].getNotesData().size();
        default:
            break;
        }
        return 0;
    }

    // シーン情報のメッセージを取得
    String GetSceneInfoMsg()
    {
        const uint64 timer = Time::GetMillisec();

        int timerMod = timer % 6000;
        if (timerMod <= 2000) {
            return U"Enter:決定　BackSpace:絞り込み,戻る　F2:ソート　Esc:タイトルに戻る";
        }
        if (timerMod <= 4000) {
            return U"F10:お気に入り　F11:コンフィグ";
        }

        return U"Shift:表示モード切替　F1:オート　Ctrl+↑↓:ハイスピード変更";
    }
}
namespace ct
{
    class MusicSelectScene::Model
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
            ::InitMusics(m_musics);
            if (m_musics.size()) {
                m_audition.autoPlayAndStop(m_musics[g_selectInfo.music]);
            }
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
            m_prevAction = m_action;
            // 選択するターゲットの参照
            auto& target = ::GetSelectTarget(m_action);
            m_prevSelect = g_selectInfo.music;
            size_t size = ::GetTargetSize(m_action, m_musics);

            // ハイスピ変更
            bool isHighSpeedUpdate = m_action != Action::GenreSelect &&
                m_highSpeedDemo.update(m_data->m_scrollRate);

            m_moveSelect = isHighSpeedUpdate ? 0 : SharedLogic::MoveSelectV();
            if (m_moveSelect) {
                if (m_moveSelect < 0) {
                    ++target;
                } else {
                    target += static_cast<uint32>(size);
                    --target;
                }

                SoundManager::PlaySe(U"select");
            }
            target = size ? target % size : 0;

            // 決定ボタン
            if (PlayKey::Start().down() && size) {
                if (m_action == Action::GenreSelect) {
                    ::InitMusics(m_musics);
                    m_prevSelect = -1; //曲の変更をトリガー

                    m_action = Action::MusicSelect;
                    SoundManager::PlaySe(U"desisionSmall");
                } else if (m_action == Action::MusicSelect) {
                    m_action = Action::LevelSelect;
                    SoundManager::PlaySe(U"desisionSmall");
                } else if (m_action == Action::LevelSelect) {
                    m_isSelectedNotes = true;
                }
            }
            // キャンセルボタン
            if (PlayKey::SmallBack().down()) {
                if (m_action == Action::MusicSelect) {
                    m_action = Action::GenreSelect;
                    SoundManager::PlaySe(U"cancel");
                } else if (m_action == Action::LevelSelect) {
                    m_action = Action::MusicSelect;
                    SoundManager::PlaySe(U"cancel");
                }
            }
            //プレイモード
            if (KeyF1.down()) {
                AutoPlayManager::ChangePlayMode();
                SoundManager::PlaySe(U"desisionSmall");
            }
            //情報切り替え
            if (KeyShift.down()) {
                SoundManager::PlaySe(U"desisionSmall");
                g_selectInfo.notesInfo = g_selectInfo.notesInfo == AllNotesInfo::Level
                    ? AllNotesInfo::ClearRank : AllNotesInfo::Level;
            }
            //ソート
            if (KeyF2.down()) {
                uint32 index = m_musics[g_selectInfo.music].getIndex();
                g_selectInfo.sortMode = ::NextMode(g_selectInfo.sortMode);
                ::SortMusics(m_musics);

                for (uint32 i = 0; i < m_musics.size(); ++i) {
                    if (index == static_cast<uint32>(m_musics[i].getIndex())) {
                        g_selectInfo.music = i;
                        break;
                    }
                }
                SoundManager::PlaySe(U"desisionSmall");
            }
            // 再度indexの調整
            {
                auto& target2 = ::GetSelectTarget(m_action);
                size_t size2 = ::GetTargetSize(m_action, m_musics);
                target2 = size2 ? target2 % size2 : 0;
            }
            // 試聴
            if (m_musics.size() && !(m_action == Action::MusicSelect && (PlayKey::Up().pressed() || PlayKey::Down().pressed()))) {
                m_audition.autoPlayAndStop(m_musics[g_selectInfo.music]);
            }

            // お気に入り
            if (m_musics.size() && m_action == Action::MusicSelect || m_action == Action::LevelSelect) {
                if (KeyF10.down()) {
                    auto& selectMusic = m_musics[g_selectInfo.music];
                    bool isFavorite = !selectMusic.isFavorite();
                    selectMusic.setFavorite(isFavorite);

                    auto index = selectMusic.getIndex();
                    auto& sourceMusic = Game::Musics()[index];
                    sourceMusic.saveFavorite(isFavorite);
                    SoundManager::PlaySe(U"desisionSmall");
                }
            }
        }

        void finally()
        {
            m_audition.stop();
        }

        bool onChangeSelectMusic()
        {
            return m_prevSelect != static_cast<int32>(g_selectInfo.music);
        }

        bool onChangeAction()
        {
            return m_action != m_prevAction;
        }
        const NotesData& getSelectNotes()const
        {
            return m_musics[g_selectInfo.music][g_selectInfo.level];
        }

        const Array<MusicData>& getMusics()const
        {
            return m_musics;
        }

        Action getAction()const
        {
            return m_action;
        }

        // previous , current
        std::pair<Action, Action> getChangeAction()const
        {
            return { m_prevAction ,m_action };
        }

        int32 getMoveSelect()const
        {
            return m_moveSelect;
        }

        const HighSpeedDemo& getHighSpeedDemo()const
        {
            return m_highSpeedDemo;
        }
        bool isSelectedNotes()
        {
            return m_isSelectedNotes;
        }

        const ConfigMain& getConfig()const
        {
            return m_config;
        }
    private:
        GameData* m_data;
        Action m_action = Action::MusicSelect;
        Action m_prevAction = Action::MusicSelect;
        s3d::int32 m_moveSelect = 0;
        s3d::int32 m_prevSelect = -1;

        Array<MusicData> m_musics;
        Audition m_audition;
        HighSpeedDemo m_highSpeedDemo;
        bool m_isSelectedNotes = false;

        ConfigMain m_config;
    };
    MusicSelectScene::MusicSelectScene(const InitData& init) :
        ISceneBase(init),
        m_pModel(std::make_shared<Model>()),
        m_view(this)
    {
        m_pModel->setData(getData());
        m_pModel->init();
    }

    void MusicSelectScene::finally()
    {
        m_pModel->finally();
        if (getData().m_toScene == SceneName::Main) {
            getData().m_nowNotes = m_pModel->getSelectNotes();
        }
    }

    void MusicSelectScene::update()
    {
        m_pModel->update();

        if (m_pModel->isSelectedNotes()) {
            changeScene(SceneName::Main, 2000, CrossFade::No);
            SoundManager::PlaySe(U"desisionLarge2");
        } else if (PlayKey::BigBack().down()) {
            //戻る
            changeScene(SceneName::Title, 1000);
            SoundManager::PlaySe(U"desisionLarge");
        }

        m_view.update();
        if (m_pModel->onChangeSelectMusic()) {
            m_view.resetShaderTimer();
        }
        if (m_pModel->onChangeAction()) {
            m_view.onChangeAction();
        }
    }

    void MusicSelectScene::draw() const
    {
        m_view.draw();
        // シーン情報
        SceneInfo::Draw(::GetSceneInfoMsg());

        if (AutoPlayManager::IsAutoPlay()) {
            PutText(U"AutoPlay", Arg::center = Vec2{ Scene::Center().x, 40 });
        }
    }

    void MusicSelectScene::drawFadeIn(double t) const
    {
        if (getData().m_fromScene == SceneName::Main) {
            this->draw();
            FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
        } else {
            FadeIn(Fade::FlipPage, t, [this]() {this->draw(); }, true);
        }
    }

    void MusicSelectScene::drawFadeOut(double t) const
    {
        if (getData().m_toScene == SceneName::Main) {
            this->draw();
            FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
            const double size = EaseOut(Easing::Cubic, 300.0, 350.0, t);
            getData().m_nowNotes
                .getMusic()
                ->getTexture()
                .resized(size, size)
                .drawAt(400, 300, ColorF(1, t * t));
        } else {
            this->draw();
        }
    }

    MusicSelectScene::SelectMusicsInfo MusicSelectScene::GetSelectInfo()
    {
        return g_selectInfo;
    }

    const Array<MusicData>& MusicSelectScene::getMusics() const
    {
        return m_pModel->getMusics();
    }

    Action MusicSelectScene::getAction() const
    {
        return m_pModel->getAction();
    }

    std::pair<Action, Action> MusicSelectScene::getChangeAction() const
    {
        return m_pModel->getChangeAction();
    }

    s3d::int32 MusicSelectScene::getMoveSelect() const
    {
        return m_pModel->getMoveSelect();
    }

    const HighSpeedDemo& MusicSelectScene::getHighSpeedDemo() const
    {
        return m_pModel->getHighSpeedDemo();
    }

    const ConfigMain& MusicSelectScene::getConfig() const
    {
        return m_pModel->getConfig();
    }
}
