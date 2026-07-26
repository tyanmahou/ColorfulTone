#include <core/Data/Genre/CTCFReader.hpp>
#include <core/Data/Genre/CTCF/Evaluator/Evaluator.hpp>
#include <core/Data/Genre/CTCF/Lexer/Lexer.hpp>
#include <core/Data/Genre/CTCF/Parser/Parser.hpp>
#include <core/Data/MusicData/MusicData.hpp>
#include <Siv3D.hpp>

namespace ct
{
    class CTCFReader::Impl
    {
    private:
        ctcf::Lexer m_lexer;
        ctcf::Parser m_parser;
    public:
        Impl(const s3d::FilePath& ctfolder) :
            m_lexer(ctfolder),
            m_parser(m_lexer)
        {}
        Impl(const s3d::Arg::code_<s3d::String>& script) :
            m_lexer(script),
            m_parser(m_lexer)
        {

        }
        s3d::Optional<s3d::String> getOption(const String& option)const
        {
            return m_lexer.getOption(option);
        }
        bool expression(const NotesData& notes, ctcf::AST::INode* node)
        {
            return ctcf::Evaluator(notes).eval(node);
        }
        bool expression(const MusicData& music)
        {
            auto statement = m_parser.root()->filter;
            if (!statement) {
                return false;
            }
            if (!statement->expression) {
                return false;
            }
            auto mode = ctcf::FindIdentifierValueKind(statement->mode);
            if (mode == ctcf::IdentifierValueKind::All) {
                for (const auto& notes : music.getNotesData()) {
                    if (!this->expression(notes, statement.get())) {
                        return false;
                    }
                }
                return true;
            } else if (mode == ctcf::IdentifierValueKind::Any) {
                for (const auto& notes : music.getNotesData()) {
                    if (this->expression(notes, statement.get())) {
                        return true;
                    }
                }
                return false;
            } else if (mode == ctcf::IdentifierValueKind::None) {
                for (const auto& notes : music.getNotesData()) {
                    if (this->expression(notes, statement.get())) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }
        bool hasNotes(const MusicData& music)
        {
            if (!expression(music)) {
                // そもそも楽曲がフィルタされてる
                return false;
            }
            auto* selector = m_parser.root()->selector.get();
            for (const auto& notes : music.getNotesData()) {
                if (this->expression(notes, selector)) {
                    return true;
                }
            }
            return false;
        }
        s3d::Array<NotesData> select(const MusicData& music)
        {
            s3d::Array<NotesData> result;
            if (!expression(music)) {
                // そもそも楽曲がフィルタされてる
                return result;
            }
            auto selector = m_parser.root()->selector;
            if (!selector || !selector->expression) {
                return result;
            }
            auto mode = ctcf::FindIdentifierValueKind(selector->mode);
            if (mode == ctcf::IdentifierValueKind::All) {
                result.reserve(music.getNotesData().size());
                for (const auto& notes : music.getNotesData()) {
                    if (this->expression(notes, selector.get())) {
                        result.push_back(notes);
                    }
                }
            } else if (mode == ctcf::IdentifierValueKind::Highest) {
                NotesData highest;
                auto comp = [](const NotesData& a, const NotesData& b) -> std::strong_ordering {
                    if (auto cmp = a.getStarLv() <=> b.getStarLv(); cmp != 0) {
                        return cmp;
                    }

                    return a.getLevel() <=> b.getLevel();
                    };
                for (const auto& notes : music.getNotesData()) {
                    if (this->expression(notes, selector.get())) {
                        if (!highest.isValid() || comp(highest, notes) <= 0) {
                            highest = notes;
                        }
                    }
                }
                if (highest.isValid()) {
                    result.push_back(highest);
                }
            }
            return result;
        }

        operator bool()const
        {
            return !m_lexer.getTokens().empty();
        }
    };

    CTCFReader::CTCFReader(const s3d::FilePath& ctfolder) :
        m_pImpl(std::make_shared<Impl>(ctfolder))
    {}

    CTCFReader::CTCFReader(const s3d::Arg::code_<s3d::String>&script):
        m_pImpl(std::make_shared<Impl>(script))
    {
    }

    CTCFReader::operator bool() const
    {
        if (!m_pImpl) {
            return false;
        }
        return m_pImpl->operator bool();
    }

    s3d::Optional<s3d::String> CTCFReader::getOption(const s3d::String& option) const
    {
        return m_pImpl->getOption(option);
    }

    bool CTCFReader::expression(const MusicData& music) const
    {
        return m_pImpl->expression(music);
    }
    bool CTCFReader::hasNotes(const MusicData& music)const
    {
        return m_pImpl->hasNotes(music);
    }
    s3d::Array<NotesData> CTCFReader::select(const MusicData& music)const
    {
        return m_pImpl->select(music);
    }
}
