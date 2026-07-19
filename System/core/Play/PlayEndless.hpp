#pragma once
#include <memory>
#include <Siv3D/Fwd.hpp>
#include <core/Play/LifeGauge/LifeGaugeKind.hpp>
#include <core/Data/MusicNotesIndex.hpp>

namespace  ct
{
    class EndlessData;
    class NotesData;

	/// <summary>
	/// 選ばれた譜面
	/// </summary>
	struct EndlessSelectedNotes
	{
		/// <summary>
		/// インデックス
		/// </summary>
		MusicNotesIndex index;

		size_t musicIndex() const
		{
			return index.first;
		}
		size_t notesIndex() const
		{
			return index.second;
		}
	};

    class PlayEndless
    {
	public:
		enum State :s3d::uint8
		{
			None = 0,
			Playing = 1,
			End = 2,
		};
    public:
		PlayEndless();
        void init(const EndlessData& endless, LifeGaugeKind gauge) const;

		bool isActive() const;
        bool isStart() const;
        const EndlessData& getEndless() const;
        const NotesData& getCurrentNotes() const;
        const s3d::Array<EndlessSelectedNotes>& getSelectedNotes() const;
        size_t getTrackIndex()const;
        size_t getTrackOrder()const;

        LifeGaugeKind getGaugeKind() const;
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
    };
}