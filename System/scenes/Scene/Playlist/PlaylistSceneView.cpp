#include <scenes/Scene/Playlist/PlaylistSceneView.hpp>
#include <scenes/Scene/Playlist/PlaylistScene.hpp>
#include <core/Data/CourseData/CourseData.hpp>
#include <core/Data/EndlessData/EndlessData.hpp>
#include <core/Play/Score/CourseResult.hpp>
#include <core/Play/Score/EndlessResult.hpp>
#include <utils/Easing/EasingSequence.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Useful.hpp>
#include <Siv3D.hpp>

namespace ct
{

    class PlaylistSceneView::Impl
    {
    private:
        const PlaylistScene* const m_pScene;
        SharedDraw::DrawBGLight m_lights;
        EasingSequence m_timers;
    public:
        Impl(const PlaylistScene* const scene) :
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
            const auto& notes = playing.getNotes();
            const MusicData music = notes.getMusic();

            const auto animeTime = m_timers[U"label"].easeOut();
            // 曲情報
            SharedDraw::JacketInfo infoView;
            infoView
                .drawJucket(music.getTexture(), notes.getColor())
                .drawFavorite(music.isFavorite())
                .drawLabel(music.getTexture(), animeTime)
                .drawLabel()
                .drawLine()
                .drawTitle(music.getMusicName())
                .drawSub(notes.getLevelNameAndLevel())
                .drawDetailRight(music.getFormattedBpm());

            // track
            const auto& musics = Game::Musics();
            static const String randomName = U"？？？";
            SharedDraw::Select<PlayTrack>()
                .setLoop(false)
                .setOffset(-30.0)
                .setDrawble([&](size_t index, const PlayTrack& d, Vec2 pos) {
                    if (!d.isSecret || index <= playing.getTrackIndex()) {
                        musics[d.musicIndex].getTexture().resized(50, 50).drawAt(pos + Vec2{ 37, 30 });
                    } else {
                        TextureAsset(U"genre_random").resized(50, 50).drawAt(pos + Vec2{ 37, 30 });
                    }
                })
                .setColorCallBack([&](const PlayTrack& d) {return musics[d.musicIndex][d.notesIndex].getColor(); })
                .draw(
                    playing.playlist(),
                    playing.getTrackIndex(),
                    [&](size_t index, const PlayTrack& d)->const String& {
                        if (!d.isSecret || index <= playing.getTrackIndex()) {
                            return musics[d.musicIndex].getMusicName();
                        } else {
                            return randomName;
                        }
                    }
                );

            // 譜面情報
            SharedDraw::MemoInfo memoInfo;
            memoInfo
                .setPos(SharedDraw::MemoInfo::DefaultPos + Vec2{ 0, 300 * (1.0 - m_timers[U"memo"].easeOut()) });
            if (PlaylistScene::GetMemoInfo() == PlaylistScene::MemoInfo::Other) {
                if (const auto* coureResult = playing.getCourseResult()) {
                    memoInfo.draw(coureResult->score, false);
                }
                else if (const auto* endlessResult = playing.getEndlessResult()) {
                    memoInfo.draw(*endlessResult);
                }
            } else {
                memoInfo.draw(notes);
            }
            // 付箋
            String track = U"Track " + Format(playing.getTrackOrder());

            const String* title = m_pScene->title().has_value() ? &m_pScene->title().value() : nullptr;
            SharedDraw::Sticky(
                title,
                &track
            );
            // ハイスピ
            SharedDraw::HighSpeed(
                m_pScene->getHighSpeedDemo(),
                music,
                m_pScene->getScrollRate(),
                !m_pScene->getConfig().isActive()
            );
            if (!m_pScene->getConfig().isActive()) {
                SharedDraw::DrawPlayOptionSets();
            }
            // コンフィグ
            m_pScene->getConfig().drawWithBack();
        }
    };

    PlaylistSceneView::PlaylistSceneView(const PlaylistScene* const scene) :
        m_pImpl(std::make_shared<Impl>(scene))
    {
    }

    void PlaylistSceneView::update() const
    {
        m_pImpl->update();
    }

    void PlaylistSceneView::draw() const
    {
        m_pImpl->draw();
    }
}
