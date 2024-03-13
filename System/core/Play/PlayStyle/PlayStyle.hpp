#pragma once
#include <memory>
#include <utils/Singleton/DynamicSingleton.hpp>
#include <core/Play/PlayStyle/IPlayStyle.hpp>
#include <core/Play/PlayStyle/PlayStyleType.hpp>

namespace ct
{
    class PlayStyle :public DynamicSingleton<PlayStyle>
    {
        friend DynamicSingleton<PlayStyle>;
    public:
        void setStyle(PlayStyleType type);

        template<class T>
        void draw(const T& note, double count, double scrollRate)const
        {
            m_style->draw(note, count, scrollRate);
        }
        void drawPreview(std::function<void()> drawCallback) const
        {
            m_style->drawPreview(drawCallback);
        }
        void drawJudgeLine() const
        {
            m_style->drawJudgeLine();
        }

        void drawFrame(bool red, bool blue, bool yellow, std::function<void()>drawCallback)const
        {
            m_style->drawFrame(red, blue, yellow, drawCallback);
        }

        void drawComboAndRate(size_t combo, float rate)const
        {
            m_style->drawComboAndRate(combo, rate);
        }

        void drawTapEffect(NoteType type, NoteType baseType)const
        {
            m_style->drawTapEffect(type, baseType);
        }
        void drawJudgeEffect(Score::Judge judge, NoteType type, s3d::int64 diff)const
        {
            m_style->drawJudgeEffect(judge, type, diff);
        }
    private:
        PlayStyle();
        ~PlayStyle();
    private:
        std::unique_ptr<IPlayStyle> m_style;
    };
}
