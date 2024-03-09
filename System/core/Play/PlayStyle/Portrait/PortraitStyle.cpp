#include <core/Play/PlayStyle/Portrait/PortraitStyle.hpp>
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
    constexpr double g_width = 252;

    double GetX(NoteType type)
    {
        switch (type % 10) {
        case 1:return 400 - g_width / 3;
        case 2:return 400;
        case 3:return 400 + g_width / 3;
        case 4:return 400 + g_width / 6;
        case 5:return 400;
        case 6:return 400 - g_width / 6;
        case 7:return 400;
        case 9:return 400;
        }
        return 400;
    }
    double GetY(double count, double scrollRate = 1.0, double speed = 1.0)
    {
        constexpr double adjust = 1.5;
        return  500 - count / static_cast<double>(Object::RESOLUTION) * scrollRate * speed * adjust;
    }
    bool CanDraw(double y)
    {
        const double scale = Game::Config().m_playScale;
        return !(y < 450 - 500.0 / scale || y>150.0 + 500.0 / scale);
        //return !(y <= -50 || y > 650);
    }
}


void PortraitStyle::drawFrame(bool red, bool blue, bool yellow, std::function<void()> drawCallback) const
{
    const auto& config = Game::Config();

    {
        ScopedRenderStates2D blend(BlendState::Subtractive);
        const double w = g_width * config.m_playScale;
        RectF(400 - w / 2, 0, w, 600).draw(ColorF(0.3, 0.6));
    }
    {
        Transformer2D t2d(Mat3x2::Scale(config.m_playScale, Vec2{ 400, 500 }));

        constexpr double height = 400;
        if (red) {
            constexpr ColorF color[4] = { ColorF(1,0,0,0),ColorF(1,0,0,0),Palette::White,Palette::White };
            RectF(400 - g_width / 2, 500 - height, g_width / 3, height).draw(color);
        }
        if (blue) {
            constexpr ColorF color[4] = { ColorF(0,0,1,0), ColorF(0,0,1,0),Palette::White,Palette::White };
            RectF(400 - g_width / 6, 500 - height, g_width / 3, height).draw(color);
        }
        if (yellow) {
            constexpr ColorF color[4] = { ColorF(1,1,0,0),ColorF(1,1,0,0),Palette::White,Palette::White };
            RectF(400 + g_width / 6, 500 - height, g_width / 3, height).draw(color);
        }

        this->drawJudgeLine();

        m_effetcs[0].update();
        drawCallback();
        {
            ScopedRenderStates2D blend(BlendState::Additive);
            m_effetcs[1].update();
        }
    }
}

void PortraitStyle::drawJudgeLine() const
{
    static constexpr Color judgeLineColor(255, 165, 0, 255);

    const double Y = GetY(0);
    //判定円
    Line({ 400 - g_width / 2,Y }, { 400 + g_width / 2,Y }).draw(15, judgeLineColor);
}

void PortraitStyle::drawComboAndRate(size_t combo, float rate)
{
    const FontAsset font(FontName::Combo);

    if (combo) {
        constexpr Vec2 pos{ 65, 298 };
        TextureAsset(U"combo").draw(pos.x + 121, pos.y + 1, Palette::White);
        TextureAsset(U"combo").draw(pos.x + 120, pos.y, Palette::Black);

        String comboText = Pad(combo, { 6, L' ' });
        FontKinetic::DeleteSpace(font, comboText, Vec2{ pos.x, pos.y + 2 }, Palette::Black, Palette::White);
    }
    {
        constexpr Vec2 pos{ 550 ,300 };
        font(U"{:.2f}%"_fmt(rate)).draw(pos.x, pos.y + 1, Palette::White);
        font(U"{:.2f}%"_fmt(rate)).draw(pos.x, pos.y, Palette::Black);
    }
}

void PortraitStyle::drawTapEffect(NoteType type, NoteType baseType)
{
    ColorFx::Request(baseType);
    auto getWidth = [](NoteType type) {
        switch (type % 10) {
        case 1:
        case 2:
        case 3:
            return g_width / 3.0;
            break;
        case 4:
        case 5:
        case 6:
            return g_width * 2 / 3.0;
        case 7:
            return g_width;
        }
        return g_width;
        };

    if (type == 5 || type == 15) {
        m_effetcs[0].add<TapEffect2>(GetX(1), getWidth(1));
        m_effetcs[0].add<TapEffect2>(GetX(3), getWidth(3));
        m_effetcs[1].add<TapEffect2_2>(400 - g_width / 2);
        m_effetcs[1].add<TapEffect2_2>(400 + g_width / 2);
    } else {
        m_effetcs[1].add<TapEffect2>(GetX(type), getWidth(type));
        m_effetcs[1].add<TapEffect2_2>(GetX(type));
    }
}

void PortraitStyle::drawJudgeEffect(const String& str, NoteType type)
{
    m_effetcs[0].add<JudgeEffect>(str, Vec2{ GetX(type),400 });
}

void PortraitStyle::draw(const Bar& note, double count, double scrollRate) const
{
    double y = GetY(count, scrollRate, note.getSpeed());

    if (!CanDraw(y))
        return;

    if (note.getSpeed() != 0)
        Line({ 400 - g_width / 2,y }, { 400 + g_width / 2,y }).draw(4, ColorF(0, 0.2));
}

namespace
{
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

    String GetTextureName(NoteType type)
    {
        static const HashTable<NoteType, String> textureNameMap
        {
            { 1, U"portrait_notes1" },
            { 2, U"portrait_notes2"	 },
            { 3, U"portrait_notes3"	},
            { 4, U"portrait_notes4"	},
            { 5, U"portrait_notes5"	},
            { 6, U"portrait_notes6"	},
            { 7, U"portrait_notes7"	},
            { 9, U"portrait_notes9"	},
            { 10,U"portrait_notes10" },
            { 11,U"portrait_notes1"	},
            { 12,U"portrait_notes2"	},
            { 13,U"portrait_notes3"	},
            { 14,U"portrait_notes4"	},
            { 15,U"portrait_notes5"	},
            { 16,U"portrait_notes6"	},
            { 17,U"portrait_notes7"	},
            { 18,U"portrait_notes10" },
        };
        if (textureNameMap.count(type)) {
            return textureNameMap.at(type);
        }

        return U"portrait_notes7";
    }
    void Draw(double y, NoteType type)
    {
        //constexpr double h = 14;
        constexpr double w = g_width / 3;
        const auto& tex = TextureAsset(GetTextureName(type));
        const Color color = GetColor(type);
        switch (type) {
        case 1:
        case 11:
            tex.drawAt(400 - w, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 2:
        case 12:
            tex.drawAt(400, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 3:
        case 13:
            tex.drawAt(400 + w, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 4:
        case 14:
            tex.scaled(2, 1).drawAt(400 + w / 2, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 5:
        case 15:
            tex.scaled(1.5, 1).drawAt(400 - w * 5 / 4, y)
                .drawFrame(0.5, 0.5, color);
            tex.scaled(1.5, 1).drawAt(400 + w * 5 / 4, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 6:
        case 16:
            tex.scaled(2.0, 1).drawAt(400 - w / 2, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 7:
        case 17:
            tex.scaled(3.0, 1).drawAt(400, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 9:
            tex.scaled(3.0, 1).drawAt(400, y)
                .drawFrame(0.5, 0.5, color);
            break;
        case 10:
        case 18:
            tex.scaled(3.0, 1).drawAt(400, y)
                .drawFrame(0.5, 0.5, Palette::Black);
        default:
            break;
        }
    }

    void DrawLong(double x, double y, double w, double h, const Color& c1, const Color& c2)
    {
        RectF(x - w / 2, y, w, h).draw(ColorF(0, 0.5));
        RectF(x - w / 2 + 5, y, w - 10, h + 5).draw({ c1,c1,c2,c2 });
    }
}
void PortraitStyle::draw(const Note& note, double count, double scrollRate) const
{

    const NoteType type = note.getType();

    if (type >= 11 && type <= 17 && note.isFirstTap())
        count = 0;

    double y = GetY(count, scrollRate, note.getSpeed());

    if (!CanDraw(y))
        return;

    Draw(y, type);
}


void PortraitStyle::draw(const LongNote& note, double count, double scrollRate) const
{
    const auto parent = note.getParent();

    const NoteType type = note.getType();

    const Color& color = note.getColor();

    double y = GetY(count, scrollRate, note.getSpeed());


    const double nowCount = note.getDrawCount() - count;
    auto pCount = parent->getDrawCount() - nowCount;
    if (parent->isFirstTap())
        pCount = 0;

    if (pCount > count)
        return;

    double pY = GetY(pCount, scrollRate, parent->getSpeed());


    if (!CanDraw(pY) && !CanDraw(y))
        return;


    Color c1 = color;
    Color c2 = GetColor(type);


    if (type == 5 || type == 15) {
        DrawLong(400 - g_width / 2, pY, 21, y - pY, c1, c2);
        DrawLong(400 + g_width / 2, pY, 21, y - pY, c1, c2);
    } else {
        DrawLong(GetX(type), pY, 21, y - pY, c1, c2);
    }
}

void PortraitStyle::draw(const RepeatNote& note, double count, double scrollRate) const
{
    if (note.isFirstTap())
        count = 0;

    double y = GetY(count, scrollRate, note.getSpeed());

    if (!CanDraw(y))
        return;

    Draw(y, 10);
}

void PortraitStyle::draw(const RepeatEnd& note, double count, double scrollRate) const
{
    const auto parent = note.getParent();

    double y = ::GetY(count, scrollRate, note.getSpeed());

    const double nowCount = note.getDrawCount() - count;
    auto pCount = parent->getDrawCount() - nowCount;
    if (parent->isFirstTap())
        pCount = 0;
    auto pY = GetY(pCount, scrollRate, parent->getSpeed());

    if (!CanDraw(pY) && !CanDraw(y))
        return;


    Color c1 = HSV(static_cast<s3d::int32>(count / 20) % 360, 0.5, 1);
    Color c2 = HSV((static_cast<s3d::int32>(count / 20) + 72) % 360, 0.5, 1);

    DrawLong(400, pY, 140, y - pY, c1, c2);

}
