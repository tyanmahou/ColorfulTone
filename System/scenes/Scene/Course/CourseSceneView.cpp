#include <scenes/Scene/Course/CourseSceneView.hpp>
#include <scenes/Scene/Course/CourseScene.hpp>
#include <Useful.hpp>
#include <utils/Easing/EasingSequence.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Siv3D.hpp>

namespace ct
{

    class CourseSceneView::Impl
    {
    private:
        const CourseScene* const m_pScene;
        SharedDraw::DrawBGLight m_lights;
        EasingSequence m_timers;
    public:
        Impl(const CourseScene* const scene) :
            m_pScene(scene)
        {
            m_timers.regist(U"label", { 0,1,Easing::Circ, 1000 });
            m_timers.regist(U"memo", { 0,1,Easing::Back, 1000 });
        }

        void update()
        {
            m_timers.update();
            m_lights.update();
        }

        void draw() const
        {
            TextureAsset(U"canvasBg").draw();
            m_lights.draw();

            const auto& playing = m_pScene->getPlay();
            const auto& notes = playing.getCurrentNotes();
            const MusicData music =notes.getMusic();

            const auto animeTime = m_timers[U"label"].easeOut();
            // ジャケ絵描画
            const Vec2 pos{ Constants::JacketCenter, 250 };
            const Vec2 size{ 310,310 };
            RectF(pos - size / 2.0, size).draw(notes.getColor());
            music
                .getTexture()
                .resized(size)
                .rotated(Math::ToRadians(-7.0))
                .drawAt(Constants::JacketCenter, 250);
            if (music.isFavorite()) {
                TextureAsset(U"favorite").drawAt(pos + Vec2{ 155, -155 });
            }
            // 曲情報
            SharedDraw::JacketInfo infoView;
            infoView
                .drawLabel(music.getTexture(), animeTime)
                .drawLabel()
                .drawLine()
                .drawTitle(music.getMusicName())
                .drawSub(notes.getLevelNameAndLevel())
                .drawDetailRight(music.getFormattedBpm());

            // track
            const auto& musics = Game::Musics();
            static const String randomName = U"？？？";
            SharedDraw::Select<CourceSelectedNotes>()
                .setLoop(false)
                .setOffset(-30.0)
                .setDrawble([&](size_t index, const CourceSelectedNotes& d, Vec2 pos) {
                    if (!d.isSecret || index <= playing.getTrackIndex()) {
                        musics[d.musicIndex()].getTexture().resized(50, 50).drawAt(pos + Vec2{ 37, 30 });
                    } else {
                        TextureAsset(U"genre_random").resized(50, 50).drawAt(pos + Vec2{ 37, 30 });
                    }
                })
                .setColorCallBack([&](const CourceSelectedNotes& d) {return musics[d.musicIndex()][d.notesIndex()].getColor(); })
                .draw(
                    playing.getSelectedNotes(),
                    playing.getTrackIndex(),
                    [&](size_t index, const CourceSelectedNotes& d)->const String& {
                        if (!d.isSecret || index <= playing.getTrackIndex()) {
                            return musics[d.musicIndex()].getMusicName();
                        } else {
                            return randomName;
                        }
                    }
                 );

            // 譜面情報
            SharedDraw::MemoInfo memoInfo;
            memoInfo
                .setPos(SharedDraw::MemoInfo::DefaultPos + Vec2{ 0, 300 * (1.0 - m_timers[U"memo"].easeOut()) });
            if (CourseScene::GetMemoInfo() == CourseScene::MemoInfo::Course) {
                memoInfo.draw(playing.getScore(), false);
            } else {
                memoInfo.draw(notes);
            }
            // 付箋
            String track = U"Track " + Format(playing.getTrackOrder());
            SharedDraw::Sticky(
                &playing.getCourse().getTitle(),
                &track
            );
            // ハイスピ
            SharedDraw::HighSpeed(
                m_pScene->getHighSpeedDemo(),
                music,
                m_pScene->getScrollRate(),
                !m_pScene->getConfig().isActive()
            );
            // コンフィグ
            if (m_pScene->getConfig().isActive()) {
                Scene::Rect().draw(ColorF(0, 0.8));
                m_pScene->getConfig().draw();
            }
        }
    };

    CourseSceneView::CourseSceneView(const CourseScene* const scene) :
        m_pImpl(std::make_shared<Impl>(scene))
    {}

    void CourseSceneView::update() const
    {
        m_pImpl->update();
    }

    void CourseSceneView::draw() const
    {
        m_pImpl->draw();
    }
}
