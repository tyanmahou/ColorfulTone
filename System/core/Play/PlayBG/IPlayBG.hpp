#pragma once

namespace ct
{
	class NotesData;

	class IPlayBG
	{
	public:
		IPlayBG() = default;
		virtual ~IPlayBG() = default;
		virtual void init(const NotesData& notes) = 0;
		virtual void apply(double count = 0)const = 0;
	};
}