#include <core/Play/PlayStyle/NormalArc/NormalArcStyle.hpp>
#include <Useful.hpp>

#include <core/Object/Note/Note.hpp>
#include <core/Object/Bar/Bar.hpp>
#include <core/Object/LongNote/LongNote.hpp>
#include <core/Object/RepeatNote/RepeatNote.hpp>

#include <core/Play/Effects/TapEffect.hpp>
#include <core/Play/Effects/JudgeEffect.hpp>
#include <Siv3D.hpp>

#include <core/Play/PlayMusicGame.hpp>
#include <core/Play/ColorFx/ColorFx.hpp>
namespace
{
    using namespace ct;
    using s3d::Math::Pi;
    using s3d::Math::TwoPi;

    Color GetColor(NoteType type)
    {
        switch (type % 10) {
        case 1:return Palette::Red;
        case 2:return Palette::Blue;
        case 3:return Palette::Yellow;
        case 4:return Palette::Green;
        case 5:return Palette::Orange;
        case 6:return Palette::Purple;
        case 7:return Palette::Black;
        case 9:return Palette::Gray;
        }
        return Palette::Black;
    }
    const Vec2 GetPos(double angle, double count, double scrollRate, double scrollSpeed)
    {
        const double c = Cos(angle);
        const double s = Sin(angle);

        const double r = 40 + count / static_cast<double>(Object::RESOLUTION) * scrollRate * scrollSpeed;
        Vec2 pos;
        pos.x = 400 + r * c;
        pos.y = 300 + r * s;
        return pos;
    }
    //--------------------------------------------------------------------------------
    //関数：GetAngle
    //--------------------------------------------------------------------------------
    //概要：ノーツが来る角度を取得
    //--------------------------------------------------------------------------------
    double GetAngle(NoteType type)
    {
        static const HashTable<NoteType, double> angleMap
        {
            { 1,7 * Pi / 6 },
            { 2, Pi / 2 },
            { 3,11 * Pi / 6 },
            { 4,Pi / 6 },
            { 5,3 * Pi / 2 },
            { 6,5 * Pi / 6 },
            { 7,0 },
        };
        if (angleMap.count(type % 10)) {
            return angleMap.at(type % 10);
        }

        return 0.0;
    }
}

namespace ct
{
    void NormalArcStyle::drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const
    {
        const auto& config = Game::Config();
        auto render = [&] {
            Transformer2D t2d(Mat3x2::Scale(config.m_playScale, Vec2{ 400,300 }));

            if (red)
                TextureAsset(U"center_redlight").drawAt(400, 300);

            if (blue)
                TextureAsset(U"center_bluelight").drawAt(400, 300);

            if (yellow)
                TextureAsset(U"center_yellowlight").drawAt(400, 300);

            TextureAsset(U"center_base").drawAt(400, 300);

            //判定円
            this->drawJudgeLine();

            m_effetcs[0].update();
            drawCallback();
            };
        if (config.m_isCirleCut) {
            const Circle mask(400, 300, 300);
            {
                ScopedRenderStates2D blend(BlendState::Subtractive);
                mask.draw(ColorF(0.3, 0.3));
            }
            auto scopedMask = Shaders::Mask().equal([&] {
                mask.draw();
                });
            render();
        } else {
            render();
        }
        {
            ScopedRenderStates2D blend(BlendState::Additive);
            m_effetcs[1].update();
        }
    }

    void NormalArcStyle::drawJudgeLine() const
    {
        static constexpr Color judgeLineColor(255, 165, 0, 255);

        //判定円
        Circle(400, 300, 40).drawFrame(2, 2, judgeLineColor);
    }

    void NormalArcStyle::drawComboAndRate(size_t combo, float rate)
    {
        const FontAsset font(FontName::Combo);

        if (combo) {
            const s3d::int32 x = 115;
            TextureAsset(U"combo").draw(x + 121, 299, Palette::White);
            TextureAsset(U"combo").draw(x + 120, 298, Palette::Black);

            String comboText = Pad(combo, { 6, L' ' });
            FontKinetic::DeleteSpace(font, comboText, Vec2{ x, 300 }, Palette::Black, Palette::White);
        }

        font(U"{:.2f}%"_fmt(rate)).draw(501, 301, Palette::White);
        font(U"{:.2f}%"_fmt(rate)).draw(500, 300, Palette::Black);
    }

    void NormalArcStyle::drawTapEffect(NoteType type, NoteType baseType)
    {
        ColorFx::Request(baseType);
        if (type == 9) {
            m_effetcs[0].add<TapEffect>(0, 9);
            m_effetcs[1].add<TapEffect2_2>(GetPos(0, 0, 0, 0));
            m_effetcs[0].add<TapEffect>(Pi, 9);
            m_effetcs[1].add<TapEffect2_2>(GetPos(Pi, 0, 0, 0));
        } else if (type == 7 || type == 17) {
            m_effetcs[0].add<TapEffect>(1 * Pi / 6, 7);
            m_effetcs[1].add<TapEffect2_2>(GetPos(1 * Pi / 6, 0, 0, 0));
            m_effetcs[0].add<TapEffect>(9 * Pi / 6, 7);
            m_effetcs[1].add<TapEffect2_2>(GetPos(9 * Pi / 6, 0, 0, 0));
            m_effetcs[0].add<TapEffect>(5 * Pi / 6, 7);
            m_effetcs[1].add<TapEffect2_2>(GetPos(5 * Pi / 6, 0, 0, 0));
        } else if (type == 18 || type == 10) {
            m_effetcs[0].add<TapEffect>(3 * Pi / 6, 7);
            m_effetcs[1].add<TapEffect2_2>(GetPos(3 * Pi / 6, 0, 0, 0));
            m_effetcs[0].add<TapEffect>(7 * Pi / 6, 7);
            m_effetcs[1].add<TapEffect2_2>(GetPos(7 * Pi / 6, 0, 0, 0));
            m_effetcs[0].add<TapEffect>(11 * Pi / 6, 7);
            m_effetcs[1].add<TapEffect2_2>(GetPos(1 * Pi / 6, 0, 0, 0));
        } else {
            m_effetcs[0].add<TapEffect>(GetAngle(type), type % 10);
            m_effetcs[1].add<TapEffect2_2>(GetPos(GetAngle(type), 0, 0, 0));
        }
    }

    void NormalArcStyle::drawJudgeEffect(const String& str, NoteType type)
    {
        if (type == 9) {
            m_effetcs[0].add<JudgeEffect>(str, GetPos(3 * Pi / 2, 2400, 1.0f, 1.0));
        } else if (type == 7 || type == 17 || type == 10) {
            m_effetcs[0].add<JudgeEffect>(str, GetPos(3 * Pi / 2, 2400, 1.0f, 1.0));
        } else {
            m_effetcs[0].add<JudgeEffect>(str, GetPos(GetAngle(type), 2400, 1.0f, 1.0));
        }

    }

    void NormalArcStyle::draw(const Bar& note, double count, double scrollRate) const
    {
        double x = 40 + count / static_cast<double>(Object::RESOLUTION) * scrollRate * note.getSpeed();

        if (x <= 0 || x > 1000)
            return;
        Circle(400, 300, x).drawFrame(2, 2, ColorF(0, 0.2));
    }

    void NormalArcStyle::draw(const Note& note, double count, double scrollRate)const
    {
        const NoteType type = note.getType();
        const double angle = GetAngle(type);
        const double& speed = note.getSpeed();

        if (type >= 11 && note.isFirstTap())
            count = 0;

        double r = 40 + count / static_cast<double>(Object::RESOLUTION) * scrollRate * speed;

        if (r <= 0 || r > 1000)
            return;

        auto playScale = Game::Config().m_playScale;

        if (type == 7 || type == 17) {

            auto arcAngle = -TwoPi / 3.0 * Min(1.0 - (r - 40) / (400 / playScale), 1.0);
            {
                Circle(400, 300, r)
                    .drawArc(-Pi / 3.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(-Pi / 3.0, arcAngle, 5, 5, GetColor(type));
            }
            {
                Circle(400, 300, r)
                    .drawArc(-5.0 * Pi / 3.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(-5.0 * Pi / 3.0, arcAngle, 5, 5, GetColor(type));
            }
            {
                Circle(400, 300, r)
                    .drawArc(-Pi, arcAngle, 6, 6, Palette::Black)
                    .drawArc(-Pi, arcAngle, 5, 5, GetColor(type));
            }
            return;
        } else if (type == 18) {
            Color c1 = HSV(static_cast<s3d::int32>(count / 20) % 360, 0.5, 1);

            auto arcAngle = TwoPi / 3.0 * Min(1.0 - (r - 40) / (400 / playScale), 1.0);
            {
                Circle(400, 300, r)
                    .drawArc(0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(0, arcAngle, 5, 5, c1);
            }
            {
                Circle(400, 300, r)
                    .drawArc(-4.0 * Pi / 3.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(-4.0 * Pi / 3.0, arcAngle, 5, 5, c1);
            }
            {
                Circle(400, 300, r)
                    .drawArc(-2.0 * Pi / 3.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(-2.0 * Pi / 3.0, arcAngle, 5, 5, c1);
            }
            return;
        } else if (type == 9) {
            auto arcAngle = TwoPi / 3.0;
            {
                Circle(400, 300, r)
                    .drawArc(TwoPi / 12.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(TwoPi / 12.0, arcAngle, 5, 5, Palette::White);
            }
            {
                Circle(400, 300, r)
                    .drawArc(Pi + TwoPi / 12.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(Pi + TwoPi / 12.0, arcAngle, 5, 5, Palette::White);
            }
            return;
        }
        auto startAngle = angle + TwoPi / 12.0;
        auto arcAngle = TwoPi / 3.0;
        //if ((type % 10) >= 4) {
        //	startAngle = angle - TwoPi / 12.0;
        //	arcAngle = 2 * TwoPi / 3.0;
        //}
        Circle(400, 300, r)
            .drawArc(startAngle, arcAngle, 6, 6, Palette::Black)
            .drawArc(startAngle, arcAngle, 5, 5, GetColor(type));
    }

    namespace
    {
        template<class ColorType>
        void DrawLongTail(double count, double pCount, double speed, double pSpeed, double scrollRate, double offset, const ColorType& color, bool isReturn = false)
        {
            const double scroll = isReturn ? -scrollRate : scrollRate;
            const Vec2 pos = ::GetPos((offset + (pCount * scroll) / 10000.0) * Pi / 6, count, scrollRate, speed);
            const Vec2 pPos = ::GetPos((offset + (pCount * scroll) / 10000.0) * Pi / 6, pCount, scrollRate, pSpeed);

            if constexpr (std::is_array_v<ColorType>) {
                Line(pos, pPos).draw(8, ColorF(0, 0.5)).draw(4, color[0], color[1]);
            } else {
                Line(pos, pPos).draw(8, ColorF(0, 0.5)).draw(4, color);
            }
        }
    }
    void NormalArcStyle::draw(const LongNote& note, double count, double scrollRate) const
    {
        const auto parent = note.getParent();
        const auto angle = ::GetAngle(parent->getType());
        const auto& speed = note.getSpeed();
        const NoteType type = note.getType();
        const Color& color = note.getColor();
        Vec2 pos = ::GetPos(angle, count, scrollRate, speed);

        const double nowCount = note.getDrawCount() - count;
        auto pCount = parent->getDrawCount() - nowCount;
        if (parent->isFirstTap())
            pCount = 0;
        auto pPos = ::GetPos(angle, pCount, scrollRate, parent->getSpeed());

        if (!Note::CanDraw(pPos) && !Note::CanDraw(pos))
            return;

        if (type == 17) {
            ::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 1, color);
            ::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 5, color);
            ::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 9, color);
            return;
        }
        Line(pos, pPos).draw(8, ColorF(0, 0.5)).draw(4, color);
    }

    void NormalArcStyle::draw(const RepeatNote& note, double count, double scrollRate) const
    {
        auto drawCount = count;
        if (note.isFirstTap())
            count = 0;

        auto speed = note.getSpeed();
        double r = 40 + count / static_cast<double>(Object::RESOLUTION) * scrollRate * speed;

        if (r <= 0 || r > 1000)
            return;

        {
            Color c1 = HSV(static_cast<s3d::int32>(drawCount / 20) % 360, 0.5, 1);

            auto playScale = Game::Config().m_playScale;
            auto arcAngle = TwoPi / 3.0 * Min(1.0 - (r - 40) / (400 / playScale), 1.0);
            {
                Circle(400, 300, r)
                    .drawArc(0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(0, arcAngle, 5, 5, c1);
            }
            {
                Circle(400, 300, r)
                    .drawArc(-4.0 * Pi / 3.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(-4.0 * Pi / 3.0, arcAngle, 5, 5, c1);
            }
            {
                Circle(400, 300, r)
                    .drawArc(-2.0 * Pi / 3.0, arcAngle, 6, 6, Palette::Black)
                    .drawArc(-2.0 * Pi / 3.0, arcAngle, 5, 5, c1);
            }
        }
    }

    void NormalArcStyle::draw(const RepeatEnd& note, double count, double scrollRate) const
    {
        const auto parent = note.getParent();
        const auto angle = ::GetAngle(parent->getType());
        const auto speed = note.getSpeed();
        const Vec2 pos = ::GetPos(angle, count, scrollRate, speed);

        const double nowCount = note.getDrawCount() - count;
        auto pCount = parent->getDrawCount() - nowCount;
        if (parent->isFirstTap())
            pCount = 0;
        auto pPos = GetPos(angle, pCount, scrollRate, parent->getSpeed());

        if (!Note::CanDraw(pPos) && !Note::CanDraw(pos))
            return;

        {
            Color c1 = HSV(static_cast<s3d::int32>(count / 10) % 360, 0.5, 1);
            Color c2 = HSV((static_cast<s3d::int32>(count / 10) + 72) % 360, 0.5, 1);

            const Color(&color)[2] = { c1,c2 };
            ::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 3, color, true);
            ::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 7, color, true);
            ::DrawLongTail(count, pCount, speed, parent->getSpeed(), scrollRate, 11, color, true);
        }
    }

}