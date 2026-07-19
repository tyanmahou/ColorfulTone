#include <scenes/Scene/Endless/EndlessSceneView.hpp>
#include <scenes/Scene/Endless/EndlessScene.hpp>
#include <core/Data/EndlessData/EndlessData.hpp>
#include <Useful.hpp>
#include <utils/Easing/EasingSequence.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <Siv3D.hpp>

namespace ct
{

    class EndlessSceneView::Impl
    {
    private:
        const EndlessScene* const m_pScene;
        SharedDraw::DrawBGLight m_lights;
        EasingSequence m_timers;
    public:
        Impl(const EndlessScene* const scene) :
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
            SharedDraw::Select<EndlessSelectedNotes>()
                .setLoop(false)
                .setOffset(-30.0)
                .setDrawble([&](size_t index, const EndlessSelectedNotes& d, Vec2 pos) {
                    if (index <= playing.getTrackIndex()) {
                        musics[d.musicIndex()].getTexture().resized(50, 50).drawAt(pos + Vec2{ 37, 30 });
                    } else {
                        TextureAsset(U"genre_random").resized(50, 50).drawAt(pos + Vec2{ 37, 30 });
                    }
                })
                .setColorCallBack([&](const EndlessSelectedNotes& d) {return musics[d.musicIndex()][d.notesIndex()].getColor(); })
                .draw(
                    playing.getSelectedNotes(),
                    playing.getTrackIndex(),
                    [&](size_t index, const EndlessSelectedNotes& d)->const String& {
                        if (index <= playing.getTrackIndex()) {
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
            if (EndlessScene::GetMemoInfo() == EndlessScene::MemoInfo::Endless) {
                memoInfo.draw(playing.getEndless(), playing.getGaugeKind());
            } else {
                memoInfo.draw(notes);
            }
            // 付箋
            String track = U"Track " + Format(playing.getTrackOrder());
            SharedDraw::Sticky(
                &playing.getEndless().getTitle(),
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

    EndlessSceneView::EndlessSceneView(const EndlessScene* const scene) :
        m_pImpl(std::make_shared<Impl>(scene))
    {
    }

    void EndlessSceneView::update() const
    {
        m_pImpl->update();
    }

    void EndlessSceneView::draw() const
    {
        m_pImpl->draw();
    }
}
