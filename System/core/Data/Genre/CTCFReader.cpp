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
            m_parser(m_lexer.getTokens())
        {}
        Impl(const s3d::Arg::code_<s3d::String>& script) :
            m_lexer(script),
            m_parser(m_lexer.getTokens())
        {

        }
        s3d::Optional<s3d::String> getOption(const String& option)const
        {
            return m_lexer.getOption(option);
        }
        GenreFilterEvalMode getEvalMode() const
        {
            auto eval = this->getOption(U"EVAL").value_or(U"ANY");
            return eval == U"ALL" ? GenreFilterEvalMode::All : GenreFilterEvalMode::Any;
        }
        bool expression(const NotesData& notes)
        {
            return ctcf::Evaluator(notes).eval(m_parser.root().get());
        }
        bool expression(const MusicData& music)
        {
            if (getEvalMode() == GenreFilterEvalMode::All) {
                if (music.getNotesData().empty()) {
                    return false;
                }
                for (const auto& notes : music.getNotesData()) {
                    if (!this->expression(notes)) {
                        return false;
                    }
                }
                return true;
            } else {
                for (const auto& notes : music.getNotesData()) {
                    if (this->expression(notes)) {
                        return true;
                    }
                }
            }
            return false;
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
        return m_pImpl->operator bool();
    }

    s3d::Optional<s3d::String> CTCFReader::getOption(const s3d::String& option) const
    {
        return m_pImpl->getOption(option);
    }

    GenreFilterEvalMode CTCFReader::getEvalMode() const
    {
        return m_pImpl->getEvalMode();
    }

    bool CTCFReader::expression(const MusicData& music) const
    {
        return m_pImpl->expression(music);
    }
    bool CTCFReader::expression(const NotesData& notes) const
    {
        return  m_pImpl->expression(notes);
    }
}
