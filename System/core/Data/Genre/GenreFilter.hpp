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

    class IGenreFilterImpl
    {
    public:
        virtual ~IGenreFilterImpl() = default;

        virtual void onFilterStart([[maybe_unused]] const s3d::Array<MusicData>& musics) {};
        [[nodiscard]] virtual bool filter(const MusicData& music) const = 0;
        [[nodiscard]] virtual bool filter(const NotesData& notes) const = 0;
    };

    class GenreFilter
    {
    public:
        explicit GenreFilter(std::unique_ptr<IGenreFilterImpl>&& filter);

        explicit GenreFilter(std::function<bool(const MusicData&)>&& filter);
        GenreFilter(std::function<bool(const NotesData&)>&& filter, GenreFilterEvalMode eval);

        void onFilterStart(const s3d::Array<MusicData>& musics) const;

        [[nodiscard]] bool operator()(const MusicData& music) const;
        [[nodiscard]] bool operator()(const NotesData& notes) const;
    private:
        std::unique_ptr<IGenreFilterImpl> m_pImpl;
    };
}