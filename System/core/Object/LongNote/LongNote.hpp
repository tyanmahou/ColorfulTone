#pragma once
#include <core/Object/Note/Note.hpp>
#include <memory>

namespace ct
{
    class LongNote :public Object
    {
    private:
        const NoteType m_baseType;
        NoteType m_type;
        s3d::Color m_color;
    protected:
        std::shared_ptr<Note> m_parent;
        std::function<bool(void)> m_judge;
        const double m_scrollSpeed;
        void perfect();
        void miss();

    public:
        LongNote(s3d::int64 timingSample, const NoteType type, double firstCount, double speed, std::shared_ptr<Note>& parent);
        virtual ~LongNote() = default;
        virtual void init() override;
        virtual bool update(const PlaybackState& state)override;
        virtual void diffDraw(double count, double scrollRate)const override;
        const double& getSpeed()const { return m_scrollSpeed; }
        const  NoteType getType()const { return m_type; }

        const s3d::Color& getColor()const { return m_color; }
        std::shared_ptr<Note> getParent()const
        {
            return m_parent;
        }
        ObjectOrder getOrder(s3d::int64 sample) const override
        {
            return ObjectOrder::Calc(this, sample);
        }

    };
}
