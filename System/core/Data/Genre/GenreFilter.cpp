#include <core/Data/Genre/GenreFilter.hpp>
#include <core/Data/MusicData/MusicData.hpp>

namespace ct
{
    GenreFilter::GenreFilter(const std::function<bool(const MusicData&)>&& filter)
    {
        m_musicFilter = std::move(filter);
        m_notesFilter = [f = m_musicFilter](const NotesData& notes) {
            return f(notes.getMusic());
        };
    }
    GenreFilter::GenreFilter(const std::function<bool(const NotesData&)>&& filter, GenreFilterEvalMode eval)
    {
        m_notesFilter = std::move(filter);
        if (eval == GenreFilterEvalMode::All) {
            m_musicFilter = [f = m_notesFilter](const MusicData& music) {
                if (music.getNotesData().isEmpty()) {
                    return false;
                }
                return music.getNotesData().all(f);
            };
        } else {
            m_musicFilter = [f = m_notesFilter](const MusicData& music) {
                return music.getNotesData().any(f);
            };
        }
    }
    bool GenreFilter::operator()(const MusicData& music) const
    {
        return m_musicFilter(music);
    }
    bool GenreFilter::operator()(const NotesData& notes) const
    {
        return m_notesFilter(notes);
    }
}
