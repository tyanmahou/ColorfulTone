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

        s3d::Optional<s3d::String> getOption(const String& option)const
        {
            return m_lexer.getOption(option);
        }
        bool expression(const NotesData& notes)
        {
            return ctcf::Evaluator(notes).eval(m_parser.root().get());
        }
        bool expression(const MusicData& music)
        {
            auto eval = this->getOption(U"EVAL").value_or(U"Any");
            if (eval == U"ALL") {
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

    CTCFReader::operator bool() const
    {
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
}
