#include <scenes/Scene/Preview/Preview.hpp>
#include <scenes/Scene/Preview/GUI/Pulldown.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <core/Analysis/Analyzer.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <scenes/Scene/Preview/GUI/Button.hpp>
#include <scenes/Scene/Preview/GUI/Slider.hpp>
#include <utils/Windows/WindowsUtil.hpp>
#include <utils/Coro/Fiber/FiberHolder.hpp>
#include <utils/Thread/Task.hpp>

namespace ct
{
    class Preview::Impl
    {
    public:
        Impl():
            m_tex(s3d::Scene::Size()),
            m_font(FontMethod::MSDF, 16, Typeface::CJK_Regular_JP)
        {
            if (Font::IsAvailable(Typeface::Icon_Awesome_Solid)) {
                m_iconFonts.emplace_back(FontMethod::MSDF, 16, Typeface::Icon_Awesome_Solid);
            }
            if (Font::IsAvailable(Typeface::Icon_MaterialDesign)) {
                m_iconFonts.emplace_back(FontMethod::MSDF, 16, Typeface::Icon_MaterialDesign);
            }
            for (const auto& iconFont : m_iconFonts) {
                m_font.addFallback(iconFont);
            }
        }
        void init()
        {
            m_scrollRate = Game::Config().m_scrollRate;

            m_config = std::make_unique<ConfigMain>();
            m_config->setActive(false);
        }
        bool updateAndDraw()
        {
            this->update();
            if (m_loading) {
                drawLoading();
            } else {
                this->draw();
                m_config->draw();
                if (m_isShowGUI) {
                    this->drawGUI();
                }
            }
            return true;
        }
    private:
        void update()
        {
            if (m_loader.resume()) {
                return;
            }
            Array<s3d::FileChange> changes;
            if (m_watcher.retrieveChanges(changes)) {
                if (m_isPlay) {
                    this->playOrStop();
                }
                if (changes.size() == 1 && m_musicData && 
                    FileSystem::RelativePath(m_musicData[m_selectNotesIndex].getFilePath()) == FileSystem::RelativePath(changes[0].path)
                    ) {
                    this->reloadNote(false);
                } else {
                    this->reload(false);
                }
            }
            if (KeyF9.down()) {
                m_isShowGUI ^= 1;
            }
            if (!m_isPlay) {
                // プレイ中じゃなければコンフィグ画面も開ける
                if (this->configUpdate(KeyF11.down())) {
                    return;
                }
            }
            //プレイモード
            if (KeyF1.down()) {
                SoundManager::PlaySe(U"desisionSmall");
                AutoPlayManager::ChangePlayMode();
            }
            if (KeyF5.down()) {
                if (!m_isPlay) {
                    this->reload();
                }
            }
            if (m_musicData) {
                if (KeyF2.down()) {
                    this->playOrStop();
                }
                if (KeyF3.down()) {
                    this->playOrStop(true);
                }
                if (KeyEscape.down()) {
                    this->stop();
                }

                if (!KeyControl.pressed() && KeyRight.pressed()) {
                    auto& s = m_musicGame.getSound();
                    int64 pos = static_cast<int64>(s.posSample() + 5000 * Scene::DeltaTime() * 60);
                    if (pos >= static_cast<int64>(s.samples())) {
                        pos = static_cast<int64>(s.samples());
                    }
                    s.seekSamples(static_cast<size_t>(pos));
                } else if (!KeyControl.pressed() && KeyLeft.pressed()) {
                    auto& s = m_musicGame.getSound();
                    int64 pos = static_cast<int64>(s.posSample() - 5000 * Scene::DeltaTime() * 60);
                    if (pos <= 0)
                        pos = 0;
                    s.seekSamples(static_cast<size_t>(pos));
                }
                m_highSpeed.update(m_scrollRate);
                m_musicGame.setScrollRate(m_scrollRate);
                if (m_isPlay) {
                    m_musicGame.update();

                    if (m_musicGame.isFinish()) {
                        this->playOrStop(true);
                    }
                } else {
                    m_musicGame.synchroCount(m_count);
                }
            }
        }
        void draw() const
        {
            PutText(U"F9:GUIの表示/非表示", Arg::center = Vec2{ 100, Scene::Height() - 20 });
            if (!m_musicData) {
                if (!m_config->isActive()) {
                    double x = 5;
                    const double topY = 70;
                    const double d = 22;
                    constexpr std::array<StringView, 6> commands{
                        U"F1:AutoPlay",
                        U"F2:現在の位置から再生/停止",
                        U"F3:曲の初めから再生/停止",
                        U"F5:更新",
                        U"F11:コンフィグ画面",
                        U"Ctrl:ハイスピの変更"
                    };
                    for (const auto& [index, str] : Indexed(commands)) {
                        PutText(String(str), Arg::topLeft = Vec2{ x, topY + d * index });
                    }
                }
                m_musicGame.playModeDraw();
                return;
            }

            if (m_isPlay) {
                m_musicGame.draw(m_isShowGUI);
            } else {
                m_musicGame.previewDraw(m_count);
                this->drawAnalizeResult();
            }
            if (m_isPlay) {
                SharedDraw::HighSpeedPlay(
                    m_highSpeed,
                    m_musicGame.getPlayNotesData(),
                    m_scrollRate
                );
            } else {
                SharedDraw::HighSpeed(
                    m_highSpeed,
                    m_musicData,
                    m_scrollRate
                );
            }
        }
        void drawGUI()
        {
            const bool enabled = m_loader.isDone();
            const double height = m_font.height() + 2 * 2;
            constexpr ColorF backColor = ColorF(0.2, 0.7);
            constexpr ColorF highlightColor = ColorF(0.9, 0.2);
            constexpr ColorF textColor = Palette::White;
            constexpr ColorF textColorDisabled = Palette::Gray;
            RectF({ 0, 0, Scene::Width(), height }).drawShadow({ 1, 1 }, 5, 0).draw(backColor);
            Vec2 pos{ 5, 0 };

            auto detailDrawer = [&](StringView str) {
                constexpr double detailFontHeight = 15;
                RectF({ Vec2{ 0, height}, Scene::Width(), m_font.height() * (detailFontHeight / m_font.fontSize()) }).draw(ColorF(0, 0.5));
                m_font(str).draw(detailFontHeight, Vec2{ 0, height });
            };
            GUI::Button button(m_font);
            button
                .setFontSize(20)
                .setMouseOverBackColor(highlightColor)
                .setTextColor(textColor, textColorDisabled)
                ;
            bool isNotPlaying = !m_config->isActive() && !m_isPlay;

            // ファイルを開く
            {
                auto region = button
                    .setEnabled(enabled && isNotPlaying)
                    .setMouseOver(std::bind(detailDrawer, U"楽曲フォルダを開く"))
                    .setOnClick(std::bind(&Impl::openProject, this))
                    .draw(U"\U000F0770", pos)
                    ;
                pos.x += region.w;
            }
            // フォルダを開く
            {
                auto region = button
                    .setEnabled(enabled && isNotPlaying && m_dirPath.has_value())
                    .setMouseOver(std::bind(detailDrawer, U"楽曲フォルダをエクスプローラーで開く"))
                    .setOnClick(std::bind(&Impl::openExplorer, this))
                    .draw(U"\U000F1781", pos)
                    ;
                pos.x += region.w + 2;
            }
            // レベル選択
            {
                auto region = GUI::Pulldown{ m_selectNotesIndex, m_isNotesListOpen }
                    .setEnabled(enabled && isNotPlaying)
                    .setFont(m_font)
                    .setMaxItemWidth(175)
                    .setTextColor(textColor, textColorDisabled)
                    .setListBackColor(backColor)
                    .setListMouseOverColor(highlightColor)
                    .setOnChange(std::bind_front(&Impl::onChangeLevel, this))
                    .draw(m_notesList, pos);
                pos.x += region.w;
                pos.x += 4;
            }
            {
                Line(pos + Vec2{ 0, 1 }, pos + Vec2{ 0, height - 1 }).draw(highlightColor);
                pos.x += 2;
            }
            // 再生ボタン
            {
                auto region = button
                    .setEnabled(enabled && !m_config->isActive() && m_musicData)
                    .setMouseOver(std::bind(detailDrawer, m_isPlay ? U"一時停止 (F2)" : U"再生 (F2)"))
                    .setOnClick(std::bind(&Impl::playOrStop, this, false))
                    .setTextColor(m_isPlay ? Palette::Red : Palette::Lightgreen, textColorDisabled)
                    .draw(m_isPlay ? U"\U000F00BC" : U"\U000F040D", pos)
                    ;
                pos.x += region.w;
            }
            // 停止ボタン
            {
                auto region = button
                    .setEnabled(enabled && !m_config->isActive() && m_musicData)
                    .setMouseOver(std::bind(detailDrawer, U"停止 (Esc)"))
                    .setOnClick(std::bind(&Impl::stop, this))
                    .setTextColor(textColor, textColorDisabled)
                    .draw(U"\U000F04DB", pos)
                    ;
                pos.x += region.w;
            }
            {
                Line(pos + Vec2{ 0, 1 }, pos + Vec2{ 0, height - 1 }).draw(highlightColor);
                pos.x += 2;
            }
            // スライダー
            {
                double d = 0;
                if (m_musicData) {
                    d = static_cast<double>(m_musicGame.getSound().posSample()) / m_musicGame.getSound().samples();
                }
                auto onChange = [&](double v) {
                    m_musicGame.getSound().seekSamples(static_cast<size_t>(v * m_musicGame.getSound().samples()));
                 };
                auto region = GUI::Slider{ d }
                    .setEnabled(enabled && !m_config->isActive() && m_musicData)
                    .setWidth(353)
                    .setHeight(height)
                    .setBaseColor(highlightColor)
                    .setOnChangeValue(std::move(onChange))
                    .draw(pos);
                pos.x += region.w;
            }
            {
                Line(pos + Vec2{ 0, 1 }, pos + Vec2{ 0, height - 1 }).draw(highlightColor);
                pos.x += 2;
            }
            // リロード
            {
                auto region = button
                    .setEnabled(enabled && isNotPlaying)
                    .setMouseOver(std::bind(detailDrawer, U"更新とフォルダを再読み込み (F5)"))
                    .setOnClick(std::bind(&Impl::reload, this, true))
                    .draw(U"\U000F0450", pos)
                    ;
                pos.x += region.w + 2;
            }
            // コンフィグ画面を開く
            {
                auto region = button
                    .setEnabled(enabled && !m_isPlay)
                    .setMouseOver(std::bind(detailDrawer, U"コンフィグ画面を開く/閉じる (F11)"))
                    .setOnClick(std::bind(&Impl::configUpdate, this, true))
                    .draw(U"\U000F0493", pos)
                    ;
                pos.x += region.w + 2;
            }
        }
        // 解析描画
        void drawAnalizeResult() const
        {
            const auto& sheet = m_musicData[m_selectNotesIndex].getSheet();
            Vec2 pos = Vec2{ 20, 320 };
            RectF(pos- Vec2{10, 10}, 200, 200).draw(ColorF(0, 0.2));
            PutText(U"[Rating]：{}"_fmt(m_analyzeResult.rating), Arg::topLeft = pos);
            pos.y += 20;
            PutText(U" - Mean：{}"_fmt(m_analyzeResult.meanRating), Arg::topLeft = pos);
            pos.y += 20;
            PutText(U" - Median：{}"_fmt(m_analyzeResult.medianRating), Arg::topLeft = pos);
            pos.y += 20;
            PutText(U" - 80%Tile：{}"_fmt(m_analyzeResult.percentile80Rating), Arg::topLeft = pos);
            pos.y += 20;
            PutText(U" - 97%Tile：{}"_fmt(m_analyzeResult.percentile97Rating), Arg::topLeft = pos);
            pos.y += 20;
            PutText(U" - Max：{}"_fmt(m_analyzeResult.maxRating), Arg::topLeft = pos);
            pos.y += 30;
            PutText(U"[Total]：{}"_fmt(sheet.getTotalNotes()), Arg::topLeft = pos);
            pos.y += 20;
            PutText(U"[Length]：{:.2f}s"_fmt(m_musicGame.getSoundLengthSec()), Arg::topLeft = pos);
        }
        void drawLoading()
        {
            m_tex.draw();
            Scene::Rect().draw(ColorF(0, 0.5));
            SharedDraw::LoadingCircle::DrawMain(ColorF(1, 0.9));
        }
        bool configUpdate(bool isOpenClose)
        {
            if (m_config->isActive()) {
                if (isOpenClose || !m_config->update()) {
                    m_config->setActive(false);
                    m_config->reset();
                    if (m_musicData) {
                        m_musicGame.reflesh();
                    }
                    SoundManager::PlaySe(U"cancel");
                }
                return true;
            } else {
                if (isOpenClose) {
                    m_config->setActive(true);
                    SoundManager::PlaySe(U"desisionSmall");
                }
            }
            return false;
        }
        bool openProject()
        {
            auto path = Dialog::SelectFolder(U"Music");
            if (path) {
                SoundManager::PlaySe(U"desisionSmall");
                m_loader.reset(std::bind(&Impl::onLoadProjectAsync, this, path));
                return true;
            } else {
                return false;
            }
        }
        bool openExplorer()
        {
            if (m_dirPath) {
                return WindowsUtil::OpenExplore(*m_dirPath);
            } else {
                return false;
            }
        }
        bool reload(bool playSe = true)
        {
            if (playSe) {
                SoundManager::PlaySe(U"desisionSmall");
            }
            m_loader.reset(std::bind(&Impl::onLoadProjectAsync, this, m_dirPath));
            m_loader.resume();
            return true;
        }
        bool reloadNote(bool playSe = true)
        {
            if (playSe) {
                SoundManager::PlaySe(U"desisionSmall");
            }
            m_loader.reset(std::bind(&Impl::onReloadNotesAsync, this));
            m_loader.resume();
            return true;
        }
        void playOrStop(bool reset = false)
        {
            if (!m_musicData) {
                return;
            }

            if (!m_isPlay) {
                if (reset) {
                    m_musicGame.getSound().seekSamples(0);
                }
                m_musicGame.getSound().play();
            } else {
                if (reset) {
                    m_musicGame.getSound().stop();
                } else {
                    m_musicGame.getSound().pause();
                }
            }

            m_isPlay ^= true;
            m_musicGame.notesInit();
        }
        void stop()
        {
            if (!m_musicData) {
                return;
            }
            m_musicGame.getSound().stop();
            m_musicGame.getSound().seekSamples(0);
            m_isPlay = false;
        }
        bool onLoadProject(const Optional<FilePath>& path)
        {
            if (!path) {
                return false;
            }
            const auto pos = m_musicGame.getSound().posSample();
            const auto genre = FileSystem::BaseName(FileSystem::ParentPath(*path));

            auto assets = FileSystem::DirectoryContents(*path);
            //iniファイルがあるか検索
            for (const auto& elm : assets) {
                if (FileSystem::Extension(elm) == U"ini") {

                    MusicData musicData(genre, *path, elm, false);
                    Array<std::pair<s3d::String, s3d::ColorF>> noteLevelNames;
                    for (auto& notes : musicData.getNotesData()) {
                        noteLevelNames.emplace_back(notes.getLevelName(), notes.getColor());
                    }
                    if (noteLevelNames.isEmpty()) {
                        return false;
                    }
                    m_musicData = musicData;
                    m_notesList = std::move(noteLevelNames);
                    if (m_notesList.isEmpty()) {
                        m_selectNotesIndex = 0;
                    } else {
                        m_selectNotesIndex %= m_notesList.size();
                    }
                    m_musicGame.init(m_musicData[m_selectNotesIndex], m_scrollRate);
                    m_analyzeResult = Analyzer::Analyze(m_musicData[m_selectNotesIndex].getSheet());
                    break;
                }
            }
            if (m_dirPath == path) {
                // 同じパスなら位置を補正
                m_musicGame.getSound().seekSamples(s3d::Clamp<size_t>(static_cast<size_t>(pos), 0, m_musicGame.getSound().samples()));
            } else {
                m_watcher = s3d::DirectoryWatcher(FilePathView(*path));
            }
            m_dirPath = path;
            return true;
        }
        template<class T>
        Coro::Fiber<void> loadingProcess(T&& func)
        {
            m_tex.fill(ColorF(1, 1));
            s3d::ScreenCapture::RequestCurrentFrame();
            co_yield{};
            m_loading = true;
            s3d::ScreenCapture::GetFrame(m_tex);
            co_await Thread::Task{ func };
            m_loading = false;
        }
        Coro::Fiber<void> onLoadProjectAsync(const Optional<FilePath>& path)
        {
            co_await loadingProcess([path, this] {return this->onLoadProject(path); });
        }
        void onReloadNotes()
        {
            if (!m_musicData) {
                return;
            }
            m_musicData[m_selectNotesIndex].reload();
            m_notesList[m_selectNotesIndex].first = m_musicData[m_selectNotesIndex].getLevelName();
            m_notesList[m_selectNotesIndex].second = m_musicData[m_selectNotesIndex].getColor();
            m_analyzeResult = Analyzer::Analyze(m_musicData[m_selectNotesIndex].getSheet());

            const auto pos = m_musicGame.getSound().posSample();
            m_musicGame.reflesh(m_musicData[m_selectNotesIndex]);
            m_musicGame.getSound().seekSamples(s3d::Clamp<size_t>(static_cast<size_t>(pos), 0, m_musicGame.getSound().samples()));
        }
        Coro::Fiber<void>  onReloadNotesAsync()
        {
            co_await loadingProcess([this] {return this->onReloadNotes(); });
        }
        bool onChangeLevel(size_t index)
        {
            if (!m_musicData) {
                return false;
            }
            SoundManager::PlaySe(U"desisionSmall");
            const auto pos = m_musicGame.getSound().posSample();
            m_musicGame.reflesh(m_musicData[index]);
            m_musicGame.getSound().seekSamples(s3d::Clamp<size_t>(static_cast<size_t>(pos), 0, m_musicGame.getSound().samples()));
            m_analyzeResult = Analyzer::Analyze(m_musicData[index].getSheet());
            return true;
        }
    private:
        DynamicTexture m_tex;
        Font m_font;
        Array<Font> m_iconFonts;

        Optional<FilePath> m_dirPath;
        PlayMusicGame m_musicGame;
        MusicData m_musicData;

        HighSpeedDemo m_highSpeed;
        double m_scrollRate = 1.0;
        bool m_isPlay = false;
        double m_count = 0;
        bool m_isShowGUI = true;

        size_t m_selectNotesIndex = 0;
        Array<std::pair<s3d::String, s3d::ColorF>> m_notesList;
        bool m_isNotesListOpen = false;

        std::unique_ptr<ConfigMain> m_config;

        s3d::DirectoryWatcher m_watcher;

        Coro::FiberHolder<void> m_loader;
        bool m_loading = false;

        AnalyzeResult m_analyzeResult;
    };
    Preview::Preview():
        m_pImpl(std::make_unique<Impl>())
    {
    }
    Preview::~Preview()
    {
    }
    void Preview::init()
    {
        m_pImpl->init();
    }
    bool Preview::updateAndDraw()
    {
        return m_pImpl->updateAndDraw();
    }
}
