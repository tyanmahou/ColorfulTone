#pragma once
#include <core/Data/Genre/CTCF/AST/INode.hpp>

namespace ct {
    class NotesData;
}

namespace ct::ctcf
{
    class Evaluator
    {
    public:
        Evaluator(const NotesData& notes):
            m_notes(notes)
        {}
        bool eval(AST::INode* node);
    private:
        const NotesData& m_notes;
    };
}