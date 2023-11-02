#pragma once
#include <Siv3D/String.hpp>

namespace ct::ctcf
{
	enum class IdentifierValueKind
	{
		Invalid,

		Bpm,
		MinBpm,
		MaxBpm,
		Artist,
		Authority,
		MusicName,
		Genre,
		Level,
		LevelName,
		Star,
		Note,
		TotalNote,
		ClearRate,
		Clear,
		AP,
		FC,
		Favorite,
	};
	enum class TokenType : size_t
	{
		Invalid,

		ParenL,
		ParenR,
		Not,
		Op,
		And,
		Or,

		// リテラル
		Number,
		String,
		IdentifierValue,

		MAX,
	};

	struct Token
	{
		s3d::String token;
		TokenType type;

		Token(const s3d::String& str);

		Token(const s3d::String& str, TokenType _type);
	};


	/// <summary>
	/// トークンタイプを探す
	/// </summary>
	TokenType FindTokenType(const s3d::String& str);
	
	IdentifierValueKind FindIdentifierValueKind(const s3d::String& str);

}