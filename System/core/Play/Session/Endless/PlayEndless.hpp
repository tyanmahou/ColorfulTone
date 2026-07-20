#pragma once
#include <memory>
#include <Siv3D/Fwd.hpp>
#include <core/Play/Session/IPlaySession.hpp>

namespace  ct
{
    class EndlessData;

    class PlayEndless : public IPlaySession
    {
    public:
		PlayEndless(const EndlessData& endless, LifeGaugeKind gauge);

	public:
		void next()  override;
		bool isEnd() const override;

		s3d::Optional<s3d::String> playlistName() const;
		LifeGaugeKind gauge() const;

		const NotesData& getNotes() const override;
		bool canForceFinishMusicGame() const override;

		const s3d::Array<PlayTrack>& playlist() const override;
		size_t getTrackIndex() const override;

		const PlayingScore& getScore() const override;
		bool keepScore() const override;
		void updateScore(const PlayingScore& score) override;

		const ScoreModel& getResult() const override;
		const CourseResult* getCourseResult() const override
		{
			return nullptr;
		}
		const EndlessResult* getEndlessResult() const override;

		s3d::StringView selectScene() const override;
		s3d::StringView playlistScene() const override;
	public:
		const EndlessData& getEndless() const;
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
    };
}