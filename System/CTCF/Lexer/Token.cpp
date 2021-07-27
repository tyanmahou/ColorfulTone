#include "Token.hpp"
#include <Siv3D.hpp>

namespace ctcf
{
	TokenType FindTokenType(const String& str)
	{
		static const std::unordered_map<String, TokenType> ids
		{
			{L"(", TokenType::ParenL},
			{L")", TokenType::ParenR},
			{L"!", TokenType::Not},
			{L"==", TokenType::Op},
			{L"!=", TokenType::Op},
			{L"<", TokenType::Op},
			{L"<=", TokenType::Op},
			{L">", TokenType::Op},
			{L">=", TokenType::Op},
			{L"in", TokenType::Op},
			{L"&&", TokenType::And},
			{L"||", TokenType::Or},

			{L"BPM", TokenType::Bpm},
			{L"MINBPM", TokenType::MinBpm},
			{L"MAXBPM", TokenType::MaxBpm},
			{L"ARTIST", TokenType::Artist},
			{L"AUTHORITY", TokenType::Authority},
			{L"MUSICNAME", TokenType::MusicName},
			{L"GENRE", TokenType::Genre},
			{L"LEVEL", TokenType::Level},
			{L"LEVELNAME", TokenType::LevelName},
			{L"STAR", TokenType::Star},
			{L"NOTE", TokenType::Note},
			{L"TOTALNOTE", TokenType::TotalNote},
			{L"CLEARRATE", TokenType::ClearRate},
			{L"CLEAR", TokenType::Clear},
			{L"FC", TokenType::FC},
			{L"AP", TokenType::AP},
			{L"FAVORITE", TokenType::Favorite},
		};
		if (ids.find(str) != ids.end()) {
			return ids.at(str);
		}
		return TokenType::Invalid;
	}
	Token::Token(const String& str) :
		token(str),
		type(FindTokenType(str))
	{}
	Token::Token(const s3d::String& str, TokenType _type) :
		token(str),
		type(_type)
	{}
}