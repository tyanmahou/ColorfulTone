#pragma once
#include <functional>
#include <variant>

namespace ct
{
    class MusicData;
    class NotesData;

    enum GenreFilterEvalMode
    {
        Any,
        All,
    };
    class GenreFilter
    {
    public:
        explicit GenreFilter(const std::function<bool(const MusicData&)>&& filter);
        GenreFilter(const std::function<bool(const NotesData&)>&& filter, GenreFilterEvalMode eval);

        [[nodiscard]] bool operator()(const MusicData& music) const;
        [[nodiscard]] bool operator()(const NotesData& notes) const;
    private:
        std::function<bool(const MusicData&)> m_musicFilter;
        std::function<bool(const NotesData&)> m_notesFilter;
    };
}