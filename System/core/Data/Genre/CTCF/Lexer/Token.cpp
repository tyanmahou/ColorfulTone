#include <core/Data/Genre/CTCF/Lexer/Token.hpp>
#include <Siv3D.hpp>

namespace ct::ctcf
{
	TokenType FindTokenType(const String& str)
	{
		static const std::unordered_map<String, TokenType> ids
		{
			{U"(", TokenType::ParenL},
			{U")", TokenType::ParenR},
			{U"!", TokenType::Not},
			{U"==", TokenType::Op},
			{U"!=", TokenType::Op},
			{U"<", TokenType::Op},
			{U"<=", TokenType::Op},
			{U">", TokenType::Op},
			{U">=", TokenType::Op},
			{U"in", TokenType::Op},
			{U"&&", TokenType::And},
			{U"||", TokenType::Or},
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
			{U"BPM",       IdentifierValueKind::Bpm},
			{U"MINBPM",    IdentifierValueKind::MinBpm},
			{U"MAXBPM",    IdentifierValueKind::MaxBpm},
			{U"ARTIST",    IdentifierValueKind::Artist},
			{U"AUTHORITY", IdentifierValueKind::Authority},
			{U"MUSICNAME", IdentifierValueKind::MusicName},
			{U"GENRE",     IdentifierValueKind::Genre},
			{U"LEVEL",     IdentifierValueKind::Level},
			{U"LEVELNAME", IdentifierValueKind::LevelName},
			{U"STAR",      IdentifierValueKind::Star},
			{U"NOTE",      IdentifierValueKind::Note},
			{U"TOTALNOTE", IdentifierValueKind::TotalNote},
			{U"CLEARRATE", IdentifierValueKind::ClearRate},
			{U"CLEAR",     IdentifierValueKind::Clear},
			{U"FC",        IdentifierValueKind::FC},
			{U"AP",        IdentifierValueKind::AP},
			{U"FAVORITE",  IdentifierValueKind::Favorite},
			{U"OFFICIAL",  IdentifierValueKind::Official},
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