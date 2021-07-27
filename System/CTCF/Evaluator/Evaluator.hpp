#pragma once
#include "../AST/INode.hpp"

class NotesData;

namespace ctcf
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