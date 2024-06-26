﻿#pragma once
#include <core/Types.hpp>
#include <functional>
#include <Siv3D/Fwd.hpp>
#include <Siv3D/Optional.hpp>

namespace ct
{
    class Note;
    class RepeatNote;
    class LongNote;
    class Bar;
    class RepeatNote;
    class RepeatEnd;

    //描画モード
    class IPlayStyle
    {
    public:
        virtual ~IPlayStyle() = default;

        virtual void drawPreview([[maybe_unused]] std::function<void()> drawCallback) const {}
        virtual void drawJudgeLine()const {}
        virtual void drawFrame([[maybe_unused]] bool red, [[maybe_unused]] bool blue, [[maybe_unused]] bool yellow, [[maybe_unused]] std::function<void()> drawCallback) const
        {
        }
        virtual void drawComboAndRate([[maybe_unused]] size_t combo, [[maybe_unused]] float rate, [[maybe_unused]] const s3d::Optional<float>& subRate)
        {
        };

        virtual void drawTapEffect([[maybe_unused]] NoteType type, [[maybe_unused]] NoteType baseType)
        {
        };
        virtual void drawJudgeEffect([[maybe_unused]] Score::Judge judge, [[maybe_unused]] NoteType type, [[maybe_unused]] s3d::int64 diff)
        {
        };

        virtual void draw(const Bar& note, double count, double scrollRate)const = 0;
        virtual void draw(const Note& note, double count, double scrollRate)const = 0;
        virtual void draw(const LongNote& note, double count, double scrollRate)const = 0;
        virtual void draw(const RepeatNote& note, double count, double scrollRate)const = 0;
        virtual void draw(const RepeatEnd& note, double count, double scrollRate)const = 0;
    };
}
