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
		};
		if (ids.find(str) != ids.end()) {
			return ids.at(str);
		}
		if (FindIdentifierValueKind(str) != IdentifierValueKind::Invalid) {
			return TokenType::IdentifierValue;
		}
		return TokenType::Invalid;
	}
	IdentifierValueKind FindIdentifierValueKind(const s3d::String& str)
	{
		static const std::unordered_map<String, IdentifierValueKind> ids
		{
			{L"BPM",       IdentifierValueKind::Bpm},
			{L"MINBPM",    IdentifierValueKind::MinBpm},
			{L"MAXBPM",    IdentifierValueKind::MaxBpm},
			{L"ARTIST",    IdentifierValueKind::Artist},
			{L"AUTHORITY", IdentifierValueKind::Authority},
			{L"MUSICNAME", IdentifierValueKind::MusicName},
			{L"GENRE",     IdentifierValueKind::Genre},
			{L"LEVEL",     IdentifierValueKind::Level},
			{L"LEVELNAME", IdentifierValueKind::LevelName},
			{L"STAR",      IdentifierValueKind::Star},
			{L"NOTE",      IdentifierValueKind::Note},
			{L"TOTALNOTE", IdentifierValueKind::TotalNote},
			{L"CLEARRATE", IdentifierValueKind::ClearRate},
			{L"CLEAR",     IdentifierValueKind::Clear},
			{L"FC",        IdentifierValueKind::FC},
			{L"AP",        IdentifierValueKind::AP},
			{L"FAVORITE",  IdentifierValueKind::Favorite},
		};
		if (ids.find(str) != ids.end()) {
			return ids.at(str);
		}
		return IdentifierValueKind::Invalid;
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