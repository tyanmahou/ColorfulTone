#include <core/Data/Genre/GenreFilter.hpp>
#include <core/Data/MusicData/MusicData.hpp>

namespace
{
    using namespace ct;

    class MusicFilter : public IGenreFilterImpl
    {
    public:
        MusicFilter(std::function<bool(const MusicData&)>&& filter):
            m_filter(std::move(filter))
        {
        }
        bool filter(const MusicData& music) const override
        {
            return m_filter(music);
        }
        bool filter(const NotesData& notes) const override
        {
            return m_filter(notes.getMusic());
        }
    private:
        std::function<bool(const MusicData&)> m_filter;
    };
    class NotesFilter : public IGenreFilterImpl
    {
    public:
        NotesFilter(std::function<bool(const NotesData&)>&& filter, GenreFilterEvalMode eval):
            m_filter(std::move(filter)),
            m_eval(eval)
        {
        }
        bool filter(const MusicData& music) const override
        {
            if (m_eval == GenreFilterEvalMode::All) {
                if (music.getNotesData().isEmpty()) {
                    return false;
                }
                return music.getNotesData().all(m_filter);
            } else {
                return music.getNotesData().any(m_filter);
            }
        }
        bool filter(const NotesData& notes) const override
        {
            return m_filter(notes);
        }
    private:
        std::function<bool(const NotesData&)> m_filter;
        GenreFilterEvalMode m_eval;
    };
}
namespace ct
{
    GenreFilter::GenreFilter(std::unique_ptr<IGenreFilterImpl>&& filter):
        m_pImpl(std::move(filter))
    {
    }
    GenreFilter::GenreFilter(std::function<bool(const MusicData&)>&& filter):
        m_pImpl(std::make_unique<MusicFilter>(std::move(filter)))
    {
    }
    GenreFilter::GenreFilter(std::function<bool(const NotesData&)>&& filter, GenreFilterEvalMode eval) :
        m_pImpl(std::make_unique<NotesFilter>(std::move(filter), eval))
    {
    }
    void GenreFilter::onFilterStart(const s3d::Array<MusicData>& musics) const
    {
        m_pImpl->onFilterStart(musics);
    }
    bool GenreFilter::operator()(const MusicData& music) const
    {
        return m_pImpl->filter(music);
    }
    bool GenreFilter::operator()(const NotesData& notes) const
    {
        return m_pImpl->filter(notes);
    }
}
