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
		Color,
		Note,
		TotalNote,
		ClearRate,
		Clear,
		AP,
		FC,
		Favorite,
		Official,

		All,
		Any,
		None,
		Highest,
	};
	enum class TokenType : size_t
	{
		Invalid,

		/// <summary>
		/// (
		/// </summary>
		ParenL,

		/// <summary>
		/// )
		/// </summary>
		ParenR,

		/// <summary>
		/// [
		/// </summary>
		LBracket,

		/// <summary>
		/// ]
		/// </summary>
		RBracket,

		Not,
		Op,
		And,
		Or,

		Colon,

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