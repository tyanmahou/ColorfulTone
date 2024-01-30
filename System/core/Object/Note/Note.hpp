#pragma once
#include <core/Object/Object.hpp>
#include <core/Data/Score/Score.hpp>
#include <functional>
#include <Siv3D/Color.hpp>

namespace ct
{
    enum ColorIndex
    {
        Red,
        Blue,
        Yellow,
    };

    class Note : public  Object
    {
    private:
        const double m_scrollSpeed;

        bool m_isClicked[3];

        const NoteType m_baseType;	//あとでenumに？
        NoteType m_type;
        s3d::Color m_color;
        std::function<bool(void)> m_judge;


        void tapUpdate(Score::Judge judge);
        void tapMiss();
    protected:

    public:
        Note(const NoteType type, double firstCount, double speed);
        virtual ~Note() = default;
        virtual void init()override;
        virtual bool update(double nowCount, double countPerFrame) override;
        virtual void diffDraw(double count, double scrollRate)const override;
        const s3d::Color& getColor()const { return m_color; }
        const  NoteType getType()const { return m_type; }
        const double& getSpeed()const { return m_scrollSpeed; }
        virtual bool isFirstTap()const;
    };
}