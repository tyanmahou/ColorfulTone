#include <scenes/Scene/EndlessSelect/EndlessSelectSceneView.hpp>
#include <scenes/Scene/EndlessSelect/EndlessSelectScene.hpp>
#include <Useful.hpp>
#include <scenes/utils/SharedDraw.hpp>
#include <scenes/Scene/Config/ConfigMain.hpp>
#include <core/Data/EndlessData/EndlessData.hpp>
#include <Siv3D.hpp>

namespace
{
    using namespace ct;
    void DrawInfo(const SharedDraw::JacketInfo& infoView, const EndlessData* pEndless)
    {
        infoView.drawLine();
        if (!pEndless) {
            return;
        }
        // ジャケ絵描画
        infoView
            .drawJucket(pEndless->getTexture(), pEndless->getColor())
            .drawTitle(pEndless->getTitle())
            .drawSub(pEndless->getDetail());
    }
    void DrawTitle()
    {
        static String title = U"ENDLESS";
        SharedDraw::Sticky(&title);
    }
}

namespace ct
{
    class EndlessSelectSceneView::Impl
    {
        const EndlessSelectScene* const m_pScene;
        SharedDraw::DrawBGLight m_lights;
        SharedDraw::WarningInfo m_warning;
    public:
        Impl(const EndlessSelectScene* const scene) :
            m_pScene(scene)
        {
        }
        void update()
        {
            m_lights.update();
        }

        void draw() const
        {
            auto select = EndlessSelectScene::GetSelectInfo();

            const auto& endless = m_pScene->getEndless();
            const EndlessData* pEndless = endless.size()
                ? &endless[select.endless] : nullptr;

            const s3d::int32 moveSelect = m_pScene->getMoveSelect();
            static EasingAB<double> easingAnime(0.0, -30.0, Easing::Linear, 100);
            if (moveSelect) {
                easingAnime.reset();
                easingAnime.start();
            }
            const double offset = easingAnime.isMoving() ?
                easingAnime.easeInOut() :
                easingAnime.getB();

            m_warning
                .setStarLv(pEndless ? pEndless->getStarLv() : StarLv::None)
                .setColor(pEndless ? pEndless->getColor() : Optional<Color>{})
                .apply([&] {
                    TextureAsset(U"canvasBg").draw();
                    m_lights.draw();

                    // リスト表示
                    {
                        SharedDraw::Select<EndlessData>()
                            .setOffset(offset)
                            .setColorCallBack([](const EndlessData& c) {
                            return c.getColor();
                                })
                            .draw(
                                endless,
                                select.endless,
                                [](const EndlessData& c)->decltype(auto) {return c.getTitle(); }
                            );
                    }


                    // コース情報表示
                    SharedDraw::JacketInfo jacketInfo;
                    jacketInfo.drawLabel();
                    ::DrawInfo(jacketInfo, pEndless);

                    // memo
                    if (pEndless) {
                        SharedDraw::MemoInfo()
                            .setPos(SharedDraw::MemoInfo::DefaultPos)
                            .draw(*pEndless, Game::Config().m_lifeGauge);
                    }
                    // ジャンル名表示
                    ::DrawTitle();

                    if (!m_pScene->getConfig().isActive()) {
                        // SharedDraw::DrawPlayContextHeader();

                        SharedDraw::DrawPlayOptionSets();
                    }

                    }
                );
            // コンフィグ
            m_pScene->getConfig().drawWithBack();
        }
    };
    EndlessSelectSceneView::EndlessSelectSceneView(const EndlessSelectScene* const scene) :
        m_pImpl(std::make_shared<Impl>(scene))
    {
    }

    void EndlessSelectSceneView::update() const
    {
        m_pImpl->update();
    }

    void EndlessSelectSceneView::draw() const
    {
        m_pImpl->draw();
    }
}
