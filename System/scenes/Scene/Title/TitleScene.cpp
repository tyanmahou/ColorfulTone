#include <scenes/Scene/Title/TitleScene.hpp>
#include <Useful.hpp>

#include <commons/Secret/Secret.hpp>
#include <scenes/utils/SharedLogic.hpp>
#include <Siv3D.hpp>

namespace
{
    void AccessHomePage()
    {
        const FilePath url = ct::Secret::HOMEPAGE_URL;
        // インターネット接続をチェック
        System::LaunchBrowser(url);
    }
}

namespace ct
{
    //--------------------------------------------------------------------------------
    //関数：コンストラクタ
    //--------------------------------------------------------------------------------

    TitleScene::Mode TitleScene::m_mode = TitleScene::Mode::GameStart;


    TitleScene::TitleScene(const InitData& init) :
        ISceneBase(init),
        m_view(this)
    {

        if (!AudioAsset(U"title").isPlaying()) {
            SoundManager::PlayBgm(U"title", 1s);
        }
        // ロードから来た場合はスタートに戻す
        if (getData().m_fromScene == SceneName::Load) {
            m_mode = Mode::GameStart;
        }
    }

    //--------------------------------------------------------------------------------
    //関数：演算子オーバーロード
    //--------------------------------------------------------------------------------

    TitleScene::Mode operator ++(TitleScene::Mode& mode)
    {
        mode = (mode == TitleScene::Mode::Exit) ? TitleScene::Mode::GameStart : TitleScene::Mode(static_cast<int>(mode) + 1);
        return mode;
    }
    TitleScene::Mode operator --(TitleScene::Mode& mode)
    {
        mode = (mode == TitleScene::Mode::GameStart) ? TitleScene::Mode::Exit : TitleScene::Mode(static_cast<int>(mode) - 1);
        return mode;
    }
    //--------------------------------------------------------------------------------
    //関数：update
    //--------------------------------------------------------------------------------

    void TitleScene::onEnterMode()
    {
        using ChangeSceneParam = struct
        {
            String name;
            int timeMillisec;
            bool crossFade;
        };
        static const std::unordered_map<Mode, ChangeSceneParam> sceneParams{
            {Mode::GameStart, {SceneName::Select, 1000, true}},
            {Mode::Course,{SceneName::CourseSelect, 1000, true}},
            {Mode::KeyConfig, {SceneName::Config, 1000, true}},
            {Mode::Download, {SceneName::Download, 1000, true}},
            {Mode::Reload, {SceneName::Load, 1000, true}},
            {Mode::Tutorial, {SceneName::Tutorial, 2000, false}},
        };

        if (m_mode == Mode::Exit) {
            System::Exit();
            return;
        } else if (m_mode == Mode::Access) {
            if (System::MessageBoxOKCancel(U"ブラウザを起動しホームページにアクセスします。") == MessageBoxResult::OK) {
                ::AccessHomePage();
            }
            return;
        } else if (m_mode == Mode::Licence) {
            if (System::MessageBoxOKCancel(U"ブラウザを起動しライセンスを表示します。") == MessageBoxResult::OK) {
                LicenseManager::ShowInBrowser();
            }
            return;
        }
        const auto& param = sceneParams.at(m_mode);
        changeScene(param.name, param.timeMillisec, CrossFade{ param.crossFade });

        const String seName = getData().m_toScene == SceneName::Tutorial ? U"desisionLarge2" : U"desisionLarge";
        SoundManager::PlaySe(seName);
    }

    void TitleScene::finally()
    {
        if (getData().m_toScene == SceneName::Select || getData().m_toScene == SceneName::Tutorial) {
            AudioAsset(U"title").stop(1s);
        }
    }

    void TitleScene::update()
    {
        bool playSe = false;
        if (int move = SharedLogic::MoveSelectV(playSe); move != 0) {
            if (playSe) {
                SoundManager::PlaySe(U"select");
            }
            if (move > 0) {
                --m_mode;
            } else if (move < 0){
                ++m_mode;
            }
        }
        if (PlayKey::Start().down()) {
            this->onEnterMode();
        }

        if (PlayKey::BigBack().down()) {
            SoundManager::PlaySe(U"select");
            m_mode = Mode::Exit;
        }

    }
    //--------------------------------------------------------------------------------
    //関数：draw
    //--------------------------------------------------------------------------------
    void TitleScene::draw()const
    {
        m_view.draw();
        SceneInfo::Draw(U"Press Enter");

        const auto&[major, minor, build, _] = Game::Version;
        FontAsset(FontName::Info)(U"Ver{}.{}.{}"_fmt(major, minor, build)).draw(10, 600 - 40 + 10);
    }

    //--------------------------------------------------------------------------------
    //関数：drawFadeIn
    //--------------------------------------------------------------------------------
    void TitleScene::drawFadeIn(double t) const
    {
        const GameData& data = getData();
        if (data.m_fromScene == SceneName::Tutorial) {
            this->draw();
            FadeIn(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
        } else if (data.m_fromScene == SceneName::Load) {
            FadeIn(Fade::FlipPage, t, [this]() {draw(); }, true);
        } else {
            FadeOut(Fade::FlipPage, t, [this]() {draw(); }, false);
        }
    }

    //--------------------------------------------------------------------------------
    //関数：drawFadeOut
    //--------------------------------------------------------------------------------
    void TitleScene::drawFadeOut(double t) const
    {
        const GameData& data = getData();
        if (data.m_toScene == SceneName::Tutorial) {
            this->draw();
            FadeOut(static_cast<FadeFunc_t>(Fade::DrawCanvas), t);
        } else {
            this->draw();
        }
    }

    TitleScene::Mode TitleScene::GetMode()
    {
        return m_mode;
    }

}
