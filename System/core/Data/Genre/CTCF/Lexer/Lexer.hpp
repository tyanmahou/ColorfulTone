#pragma once
#include <Siv3D.hpp>
#include <unordered_map>
#include <core/Data/Genre/CTCF/Lexer/Token.hpp>

namespace ct::ctcf
{
    using Options = std::unordered_map<s3d::String, s3d::String>;

	class Lexer
	{
	public:
		Lexer(const FilePath& ctfolder);
		Lexer(const s3d::Arg::code_<s3d::String>& script);
		bool load(const FilePath& ctfolder);
		bool load(const s3d::Arg::code_<s3d::String>& script);
		const Array<Token>& getTokens()const
		{
			return m_tokens;
		}
		Optional<String> getOption(const String& option)const;
	private:
		void parseLine(const String& line);

		void pushOptions(const String& option);
	private:
		Options m_options;
		Array<Token> m_tokens;
	};
}