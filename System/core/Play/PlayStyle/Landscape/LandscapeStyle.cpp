#include <core/Play/PlayStyle/Landscape/LandscapeStyle.hpp>
#include <Useful.hpp>

#include <core/Object/Note/Note.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <core/Object/LongNote/LongNote.hpp>
#include <core/Object/RepeatNote/RepeatNote.hpp>

#include <core/Play/Effects/TapEffect.hpp>
#include <core/Play/Effects/JudgeEffect.hpp>

#include <core/Play/PlayMusicGame.hpp>
#include <core/Play/ColorFx/ColorFx.hpp>
#include <core/Play/UI/PlayInfoView.hpp>

#include <Siv3D.hpp>

namespace ct
{
    namespace 
    {
        constexpr Vec2 g_pivot{ 200,200 };
        constexpr Vec2 g_pivotCore{ 100,200 };
        constexpr double g_height = 190;

        double GetX(double count, double scrollRate = 1.0, double speed = 1.0)
        {
            constexpr double adjust = 1.0;
            return  g_pivot.x + count / static_cast<double>(Object::RESOLUTION) * scrollRate * speed * adjust;
        }
        double GetY(NoteType type)
        {
            if (type == 1) {
                return g_pivot.y - 60;
            }
            if (type == 3) {
                return g_pivot.y + 60;
            }
            return g_pivot.y;
        }
        bool CanDraw(double x)
        {
            const double scale = Game::Config().m_playScale;
            constexpr double leftBase = -50;
            const double left = g_pivot.x - (g_pivot.x - leftBase) / scale;

            constexpr double rightBase = 850;
            const double right = g_pivot.x - (g_pivot.x - rightBase) / scale;

            return (left <= x && x <= right);
        }
        String GetTxetureName(NoteType type)
        {
            static const HashTable<NoteType, String> textureNameMap
            {
                { 1, U"note_red" },
                { 2, U"note_blue" },
                { 3, U"note_yellow" },
                { 4, U"note_green" },
                { 5, U"note_orange" },
                { 6, U"note_purple" },
                { 7, U"note_black" },
                { 9, U"note_white" },
                { 10,U"comet_rainbow_head" },
                { 11,U"comet_red" },
                { 12,U"comet_blue" },
                { 13,U"comet_yellow" },
                { 14,U"comet_green" },
                { 15,U"comet_orange" },
                { 16,U"comet_purple" },
                { 17,U"comet_black" },
                { 18,U"note_rainbow" },
            };
            if (textureNameMap.count(type)) {
                return textureNameMap.at(type);
            }

            return U"note_black";
        }
        void Draw(double x, NoteType type, double count = 0)
        {
            const String textureName = GetTxetureName(type);
            TextureAsset texture(textureName);

            switch (type) {
            case 1:
            case 11:
            case 2:
            case 12:
            case 3:
            case 13:
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(type % 10) });
                break;
            case 4:
            case 14:
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(2) });
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(3) });
                break;
            case 5:
            case 15:
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(1) });
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(3) });
                break;
            case 6:
            case 16:
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(1) });
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(2) });
                break;
            case 7:
            case 17:
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(1) });
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(2) });
                texture.rotated(-Math::HalfPi).drawAt(Vec2{ x, GetY(3) });
                break;
            case 9:
                Line(Vec2{ x, g_pivot.y - g_height / 2.0 }, Vec2{ x, g_pivot.y + g_height / 2.0 })
                    .draw(8, ColorF(0, 0.5))
                    .draw(4, Palette::White)
                    ;
                texture(0, 0, 60, 30).drawAt(x, g_pivot.y - g_height / 2.0 - 15);
                texture(0, 30, 60, 30).drawAt(x, g_pivot.y + g_height / 2.0 + 15);
                break;
            case 10:
            {
                TextureAsset(U"comet_rainbow_head").rotated(-Math::Pi / 4).drawAt(Vec2{ x, GetY(1) });
                TextureAsset(U"comet_rainbow_tail").rotated(Math::Pi / 4).drawAt(Vec2{ x, GetY(3) });
            }
            break;
            case 18:
            {
                Color c1 = HSV(static_cast<s3d::int32>(count / 10) % 360, 0.5, 1);
                Color c2 = HSV((static_cast<s3d::int32>(count / 10) + 72) % 360, 0.5, 1);
                const Color(&color)[2] = { c1,c2 };
                Line(Vec2{ x, GetY(1) }, Vec2{ x, GetY(3) })
                    .draw(8, ColorF(0, 0.5))
                    .draw(4, color[0], color[1])
                    ;
                TextureAsset(U"comet_rainbow_head").drawAt(Vec2{x, GetY(1)});
                TextureAsset(U"comet_rainbow_tail").drawAt(Vec2{ x, GetY(3) });
            }break;
            default:
                break;
            };
        }
        template<class ColorType>
        void DrawLongTail(double x, double pX, double y, const ColorType& color)
        {
            const Vec2 pos = Vec2{ x, y };
            const Vec2 pPos = Vec2{ pX, y };
            if constexpr (std::is_array_v<ColorType>) {
                Line(pos, pPos)
                    .draw(8, ColorF(0, 0.5))
                    .draw(4, color[0], color[1])
                    ;
            } else {
                Line(pos, pPos)
                    .draw(8, ColorF(0, 0.5))
                    .draw(4, color)
                    ;
            }
        }
        template<class ColorType>
        void DrawLong(double x, double pX, NoteType type, const ColorType& color)
        {
            const String textureName = GetTxetureName(type);
            TextureAsset texture(textureName);

            switch (type) {
            case 1:
            case 11:
            case 2:
            case 12:
            case 3:
            case 13:
                DrawLongTail(x, pX, GetY(type % 10), color);
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(type % 10) });
                break;
            case 4:
            case 14:
                DrawLongTail(x, pX, GetY(2), color);
                DrawLongTail(x, pX, GetY(3), color);
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(2) });
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(3) });
                break;
            case 5:
            case 15:
                DrawLongTail(x, pX, GetY(1), color);
                DrawLongTail(x, pX, GetY(3), color);
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(1) });
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(3) });
                break;
            case 6:
            case 16:
                DrawLongTail(x, pX, GetY(1), color);
                DrawLongTail(x, pX, GetY(2), color);
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(1) });
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(2) });
                break;
            case 7:
            case 17:
            case 9:
                DrawLongTail(x, pX, GetY(1), color);
                DrawLongTail(x, pX, GetY(2), color);
                DrawLongTail(x, pX, GetY(3), color);
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(1) });
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(2) });
                texture.rotated(Math::HalfPi).drawAt(Vec2{ x, GetY(3) });
                break;
            case 10:
            case 18:
            {
                if constexpr (std::is_array_v<ColorType>) {
                    Vec2 tl{ pX, GetY(1) };
                    Vec2 br{ x, GetY(3) };
                    RectF(tl, br - tl)
                        .stretched(5)
                        .draw(ColorF(0, 0.5))
                        .stretched(-4)
                        .draw({ color[0], color[1], color[1], color[0] })
                        ;
                }
                TextureAsset(U"comet_rainbow_head").rotated(type == 18 ? 0 : Math::Pi / 4).drawAt(Vec2{ x, GetY(1) });
                TextureAsset(U"comet_rainbow_tail").rotated(type == 18 ? 0 : -Math::Pi / 4).drawAt(Vec2{ x, GetY(3) });
            }
            default:
                break;
            };
        }

        FixedArray<double, 3> GetAngles(NoteType type)
        {

            FixedArray<double, 3> angles;
            switch (type) {
            case 1:
            case 11:
                angles.push_back(-Math::Pi / 4);
                break;
            case 2:
            case 12:
                angles.push_back(0);
                break;
            case 3:
            case 13:
                angles.push_back(Math::Pi / 4);
                break;
            case 4:
            case 14:
                angles.push_back(0);
                angles.push_back(Math::Pi / 4);
                break;
            case 5:
            case 15:
                angles.push_back(-Math::Pi / 4);
                angles.push_back(Math::Pi / 4);
                break;
            case 6:
            case 16:
                angles.push_back(-Math::Pi / 4);
                angles.push_back(0);
                break;
            case 7:
            case 17:
            case 10:
            case 18:
            case 9:
                angles.push_back(-Math::Pi / 4);
                angles.push_back(0);
                angles.push_back(Math::Pi / 4);
                break;
            default:
                break;
            };
            return angles;
        }
    }
    void LandscapeStyle::drawPreview(std::function<void()> drawCallback) const
    {
        const auto& config = Game::Config();
        {
            ScopedRenderStates2D blend(BlendState::Subtractive);
            const double h = g_height * config.m_playScale;
            RectF(0, g_pivot.y - h / 2.0, 800, h).draw(ColorF(0.3, 0.6));
        }
        {
            Transformer2D t2d(Mat3x2::Scale(config.m_playScale, g_pivot));
            this->drawJudgeLine();
            drawCallback();
        }
    }
    void LandscapeStyle::drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const
    {
        const auto& config = Game::Config();
        const double scale = config.m_playScale;
        {
            ScopedRenderStates2D blend(BlendState::Subtractive);
            const double h = g_height * scale;
            RectF(0, g_pivot.y - h / 2.0, 800, h).draw(ColorF(0.3, 0.6));
        }
        {
            Transformer2D t2d(Mat3x2::Scale(scale, g_pivot));

            //判定円
            this->drawJudgeLine();
            drawCallback();

            {
                constexpr double leftBase = -50;
                const double left = g_pivot.x - (g_pivot.x - leftBase) / scale;
                RectF(left, g_pivot.y - g_height / 2.0, (g_pivotCore.x - left), g_height).draw(ColorF(0, 1));
                double right = (g_pivotCore.x + g_pivot.x) / 2.0;
                RectF(g_pivotCore.x, g_pivot.y - g_height / 2.0, (right - g_pivotCore.x), g_height).draw({ ColorF(0, 1) , ColorF(0, 0) , ColorF(0, 0) , ColorF(0, 1) });
            }
            if (red) {
                Circle({ GetX(0), g_pivot.y - 60}, 20).draw(ColorF(1, 0, 0, 0.2));
                TextureAsset(U"center_redlight").rotated(Math::HalfPi).drawAt(g_pivotCore);
            }

            if (blue) {
                Circle({ GetX(0), g_pivot.y }, 20).draw(ColorF(0, 0, 1, 0.2));
                TextureAsset(U"center_bluelight").rotated(Math::HalfPi).drawAt(g_pivotCore);
            }

            if (yellow) {
                Circle({ GetX(0), g_pivot.y + 60 }, 20).draw(ColorF(1, 1, 0, 0.2));
                TextureAsset(U"center_yellowlight").rotated(Math::HalfPi).drawAt(g_pivotCore);            }

            TextureAsset(U"center_base").drawAt(g_pivotCore);

            m_effetcs[0].update();
            {
                ScopedRenderStates2D blend(BlendState::Additive);
                m_effetcs[1].update();
            }
        }
    }
    void LandscapeStyle::drawJudgeLine() const
    {
        static constexpr Color judgeLineColor(255, 165, 0, 255);
        double x = GetX(0);

        Circle({ x, g_pivot.y - 60 }, 20).drawFrame(4, ColorF(0, 0.2));
        Circle({x, g_pivot.y}, 20).drawFrame(4, ColorF(0, 0.2));
        Circle({ x, g_pivot.y + 60}, 20).drawFrame(4, ColorF(0, 0.2));
        Line({ x,  g_pivot.y - g_height / 2.0 }, { x,  g_pivot.y + g_height / 2.0 }).draw(4, judgeLineColor);
    }
    void LandscapeStyle::drawComboAndRate(size_t combo, float rate)
    {
        constexpr Vec2 comboPos{ g_pivotCore.x -100 , 300 + 10};
        constexpr Vec2 ratePos{ 600 ,300 + 10 };

        PlayInfoView::DrawCombo(combo, comboPos);
        PlayInfoView::DrawRate(rate, s3d::none, ratePos);
    }
    void LandscapeStyle::drawTapEffect(NoteType type, NoteType baseType)
    {
        ColorFx::Request(baseType);
        NoteType effctType = type % 10;
        if (type == 18 || type == 10) {
            effctType = 7;
        }
        m_effetcs[0].add<TapEffect>(g_pivotCore, GetAngles(type), effctType);

        switch (type) {
        case 1:
        case 11:
        case 2:
        case 12:
        case 3:
        case 13:
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(effctType)});
            break;
        case 4:
        case 14:
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(2) });
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(3) });
            break;
        case 5:
        case 15:
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(1) });
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(3) });
            break;
        case 6:
        case 16:
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(1) });
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(2) });
            break;
        case 7:
        case 17:
        case 10:
        case 18:
        case 9:
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(1) });
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(2) });
            m_effetcs[1].add<TapEffect2_2>(Vec2{ GetX(0), GetY(3) });
            break;
        default:
            break;
        };
    }
    void LandscapeStyle::drawJudgeEffect(Score::Judge judge, [[maybe_unused]]NoteType type, s3d::int64 diff)
    {
        m_effetcs[0].add<JudgeEffect>(judge, diff, g_pivotCore + Vec2{0, -120});
    }
    void LandscapeStyle::draw(const Bar& note, double count, double scrollRate) const
    {
        double x = GetX(count, scrollRate, note.getSpeed());

        if (!CanDraw(x)) {
            return;
        }
        if (note.getSpeed() != 0) {
            Line({ x,  g_pivot.y - g_height / 2.0 }, { x,  g_pivot.y + g_height / 2.0 }).draw(4, ColorF(0, 0.2));
        }
    }
    void LandscapeStyle::draw(const Note& note, double count, double scrollRate) const
    {
        const NoteType type = note.getType();

        if (type >= 11 && type <= 17 && note.isFirstTap()) {
            count = 0;
        }
        double x = GetX(count, scrollRate, note.getSpeed());

        if (!CanDraw(x)) {
            return;
        }

        Draw(x, type, count);
    }
    void LandscapeStyle::draw(const LongNote& note, double count, double scrollRate) const
    {
        const auto parent = note.getParent();
        const NoteType type = note.getType();
        const Color& color = note.getColor();

        double x = GetX(count, scrollRate, note.getSpeed());

        const double nowCount = note.getDrawCount() - count;
        auto pCount = parent->getDrawCount() - nowCount;
        if (parent->isFirstTap()) {
            pCount = 0;
        }

        if (count >= 0 && pCount > count) {
            return;
        }

        double pX = GetX(pCount, scrollRate, parent->getSpeed());

        if (count >= 0 && !CanDraw(pX) && !CanDraw(x)) {
            return;
        }

        const String textureName = GetTxetureName(type);
        TextureAsset texture(textureName);
        DrawLong(x, pX, type, color);
    }
    void LandscapeStyle::draw(const RepeatNote& note, double count, double scrollRate) const
    {
        if (note.isFirstTap()) {
            count = 0;
        }

        double x = GetX(count, scrollRate, note.getSpeed());

        if (!CanDraw(x)) {
            return;
        }

        Draw(x, 10, count);
    }
    void LandscapeStyle::draw(const RepeatEnd& note, double count, double scrollRate) const
    {
        const auto parent = note.getParent();
        double x = GetX(count, scrollRate, note.getSpeed());

        const double nowCount = note.getDrawCount() - count;
        auto pCount = parent->getDrawCount() - nowCount;
        if (parent->isFirstTap()) {
            pCount = 0;
        }

        if (count >= 0 && pCount > count) {
            return;
        }

        double pX = GetX(pCount, scrollRate, parent->getSpeed());

        if (count >=0 && !CanDraw(pX) && !CanDraw(x)) {
            return;
        }
        Color c1 = HSV(static_cast<s3d::int32>(count / 10) % 360, 0.5, 1);
        Color c2 = HSV((static_cast<s3d::int32>(count / 10) + 72) % 360, 0.5, 1);
        const Color(&color)[2] = { c1,c2 };
        DrawLong(x, pX, 10, color);
    }
}
