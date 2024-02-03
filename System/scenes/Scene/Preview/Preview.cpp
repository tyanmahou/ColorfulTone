#include <scenes/Scene/Preview/Preview.hpp>
#include <scenes/Scene/Preview/GUI/Pulldown.hpp>
#include <core/Play/PlayMusicGame.hpp>
#include <core/Play/HighSpeed/HighSpeedDemo.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class Preview::Impl
    {
    public:
        Impl():
            m_font(FontMethod::MSDF, 16, Typeface::CJK_Regular_JP),
            m_notesListPulldown(m_font, 180, {0,0})
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
        }
        bool updateAndDraw()
        {
            this->update();
            this->draw();
            this->drawGUI();
            return true;
        }
    private:
        void update()
        {
            //プレイモード
            if (KeyF1.down()) {
                SoundManager::PlaySe(U"desisionSmall");
                AutoPlayManager::ChangePlayMode();
            }
            if (KeyF5.down()) {
                if (!m_isPlay) {
                    this->onLoadProject(m_dirPath);
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
            if (!m_musicData) {
                return;
            }
            if (m_isPlay) {
                m_musicGame.draw(true);
            } else {
                m_musicGame.previewDraw(m_count);
            }
            if (m_isPlay) {
                SharedDraw::HighSpeedPlay(
                    m_highSpeed,
                    m_musicGame.getNotesData(),
                    m_scrollRate
                );
            } else {
                SharedDraw::HighSpeed(
                    m_highSpeed,
                    *m_musicData,
                    m_scrollRate
                );
            }
        }
        void drawGUI()
        {
            const double height = m_font.height() + 2 * 2;
            constexpr ColorF backColor = ColorF(0.2, 0.7);
            constexpr ColorF highlightColor = ColorF(0.9, 0.2);
            constexpr ColorF textColor = Palette::White;
            constexpr ColorF textColorEnabled = Palette::Gray;
            RectF({ 0, 0, Scene::Width(), height }).drawShadow({ 1, 1 }, 5, 0).draw(backColor);
            Vec2 pos{ 0, 0 };
            {
                const auto dtext = m_font(U"\U000F0770 開く");
                const double width = Math::Ceil(dtext.region().w) + 20;
                RectF region{ pos, {width, height} };
                if (!m_isPlay && region.mouseOver()) {
                    region.draw(highlightColor);
                    if (m_dirPath) {
                        RectF({ pos + Vec2{ 0, height}, Scene::Width(), m_font.height() * (12.0 / m_font.fontSize())}).draw(ColorF(0, 0.5));
                        m_font(*m_dirPath).draw(12, pos + Vec2{ 0, height });
                    }
                    Cursor::RequestStyle(CursorStyle::Hand);
                }
                if (!m_isPlay && region.leftClicked()) {
                    this->openProject();
                }
                dtext.drawAt(region.center(), m_isPlay ? textColorEnabled: textColor);
                pos.x += region.w;
                pos.x += 2;
            }
            {
                m_notesListPulldown.setPos(pos);
                if (!m_isPlay) {
                    if (m_notesListPulldown.update()) {
                        this->onChangeLevel();
                    }
                }
                auto region = m_notesListPulldown.draw(m_isPlay ? textColorEnabled : textColor, backColor, highlightColor);
                pos.x += region.w;
                pos.x += 2;
            }
            {
                const auto dtext = m_font(m_isPlay ? U"\U000F00BC" : U"\U000F040D");
                const double width = Math::Ceil(dtext.region(20).w) + 15;
                RectF region{ pos, {width, height} };
                if (region.mouseOver()) {
                    region.draw(highlightColor);
                    Cursor::RequestStyle(CursorStyle::Hand);
                }
                if (region.leftClicked()) {
                    this->playOrStop();
                }
                dtext.drawAt(20, region.center(), m_isPlay ? Palette::Red : Palette::Lightgreen);
                pos.x += region.w;
                pos.x += 2;
            }
            {
                const auto dtext = m_font(U"\U000F04DB");
                const double width = Math::Ceil(dtext.region().w) + 15;
                RectF region{ pos, {width, height} };
                if (region.mouseOver()) {
                    region.draw(highlightColor);
                    Cursor::RequestStyle(CursorStyle::Hand);
                }
                if (region.leftClicked()) {
                    this->stop();
                }
                dtext.drawAt(region.center(), textColor);
                pos.x += region.w;
                pos.x += 2;
            }
            {
                Line(pos + Vec2{ 0, 1 }, pos + Vec2{ 0, height - 1 }).draw(highlightColor);
                pos.x += 2;
            }
            {
                double d = 0;
                if (m_musicData) {
                    d = static_cast<double>(m_musicGame.getSound().posSample()) / m_musicGame.getSound().samples();
                }
                if (slider(d, pos, 353, height, m_musicData.has_value(), highlightColor)) {
                    m_musicGame.getSound().seekSamples(static_cast<size_t>(d * m_musicGame.getSound().samples()));
                }
                pos.x += 353;
            }
            {
                Line(pos + Vec2{ 0, 1 }, pos + Vec2{ 0, height - 1 }).draw(highlightColor);
                pos.x += 2;
            }
            {
                const auto dtext = m_font(U"\U000F0450 更新");
                const double width = Math::Ceil(dtext.region().w) + 20;
                RectF region{ pos, {width, height} };
                if (!m_isPlay && region.mouseOver()) {
                    region.draw(highlightColor);
                    Cursor::RequestStyle(CursorStyle::Hand);
                }
                if (!m_isPlay && region.leftClicked()) {
                    this->reload();
                }
                dtext.drawAt(region.center(), m_isPlay ? textColorEnabled : textColor);
                pos.x += region.w;
                pos.x += 2;
            }
            {
                
            }
        }
        bool slider(
            double& value,
            const Vec2& lt,
            const double sliderWidth,
            double height,
            const bool enabled,
            const ColorF& backColor
        )
        {
            const double width = sliderWidth;
            const RectF region{ lt, width, height };
            Vec2 center = region.center();
            const double sliderRegionX0 = (region.x + 8);
            const double sliderRegionX1 = (region.x + region.w - 8);
            const double sliderRegionW = (sliderRegionX1 - sliderRegionX0);

            const double actualSliderRegionX0 = (sliderRegionX0 + 8);
            const double actualSliderRegionX1 = (sliderRegionX1 - 8);
            const double actualSliderRegionW = (actualSliderRegionX1 - actualSliderRegionX0);

            const RectF sliderRect{ Arg::leftCenter(sliderRegionX0, center.y), sliderRegionW, 6 };
            const RoundRect baseRoundRect = sliderRect.rounded(2);
            const double previousValue = value;
            value = Saturate(value);

            const double fill = value;
            const RectF fillRect{ sliderRect.pos, sliderRect.w * fill, sliderRect.h };
            const RoundRect fillRoundRect = fillRect.rounded(2.0);

            const RectF smallRect{ Arg::center(actualSliderRegionX0 + actualSliderRegionW * fill, center.y), 16, 24 };
            const RoundRect smallRoundRect = smallRect.rounded(4.2);
            const bool mouseOver = (enabled && smallRect.mouseOver());

            if (enabled) {
                baseRoundRect.draw(backColor);
                fillRoundRect.draw({ 0.35, 0.7, 1.0 });
                smallRoundRect
                    .draw(mouseOver ? ColorF{ 0.9, 0.95, 1.0 } : ColorF{1.0})
                    .drawFrame(1, ColorF{ 0.33 });
            } else {
                baseRoundRect.draw(backColor);
                fillRoundRect.draw({ 0.75, 0.85, 1.0 });
                smallRoundRect
                    .draw(Palette::Gray)
                    .drawFrame(1, ColorF{ 0.67 });
            }

            const RectF sliderRectExtended = sliderRect.stretched(4, 12);
            if (enabled && Cursor::OnClientRect() && (sliderRectExtended.mouseOver() || smallRect.mouseOver())) {
                Cursor::RequestStyle(CursorStyle::Hand);
            }

            if (enabled && Cursor::OnClientRect() && sliderRectExtended.leftPressed()) {
                const double pos = (Cursor::PosF().x - actualSliderRegionX0);
                const double posN = Math::Saturate(pos / actualSliderRegionW);
                value = posN;
            }

            return (value != previousValue);
        }
        bool openProject()
        {
            auto path = Dialog::SelectFolder(U"Music");
            if (path) {
                return this->onLoadProject(path);
            } else {
                return false;
            }
        }
        bool reload()
        {
            return this->onLoadProject(m_dirPath);
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
                    SoundManager::PlaySe(U"desisionSmall");

                    m_musicData.emplace(genre, *path, elm);

                    Array<String> noteLevelNames;
                    for (auto& notes : m_musicData->getNotesData()) {
                        noteLevelNames.push_back(notes.getLevelName());
                    }
                    if (noteLevelNames.isEmpty()) {
                        m_musicData = none;
                        return false;
                    }
                    m_notesListPulldown.setItems(noteLevelNames);
                    m_musicGame.init((*m_musicData)[m_notesListPulldown.getIndex()], m_scrollRate);
                    break;
                }
            }
            if (m_dirPath == path) {
                // 同じパスなら位置を補正
                m_musicGame.getSound().seekSamples(s3d::Clamp<size_t>(static_cast<size_t>(pos), 0, m_musicGame.getSound().samples()));
            }
            m_dirPath = path;
            return true;
        }
        bool onChangeLevel()
        {
            if (!m_musicData) {
                return false;
            }
            SoundManager::PlaySe(U"desisionSmall");
            const auto pos = m_musicGame.getSound().posSample();
            m_musicGame.init((*m_musicData)[m_notesListPulldown.getIndex()], m_scrollRate);
            m_musicGame.getSound().seekSamples(s3d::Clamp<size_t>(static_cast<size_t>(pos), 0, m_musicGame.getSound().samples()));
            return true;
        }
    private:
        Font m_font;
        Array<Font> m_iconFonts;

        Pulldown m_notesListPulldown;

        Optional<FilePath> m_dirPath;


        PlayMusicGame m_musicGame;
        Optional<MusicData> m_musicData;

        HighSpeedDemo m_highSpeed;
        double m_scrollRate = 1.0;
        bool m_isPlay = false;
        double m_count = 0;
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
