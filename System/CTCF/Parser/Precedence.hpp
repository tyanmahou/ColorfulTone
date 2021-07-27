#pragma once
#include "../Lexer/Token.hpp"
namespace ctcf
{
	enum class Precedence
	{
		Not,
		And,
		Or,
		Lowest,
	};

	inline Precedence ToPrecedence(TokenType token)
	{
		switch (token) {
		case TokenType::Not: return Precedence::Not;
		case TokenType::And: return Precedence::And;
		case TokenType::Or: return Precedence::Or;
		default:
			return Precedence::Lowest;
		}
	}

	inline constexpr Precedence operator - (Precedence p, int i)
	{
		return static_cast<Precedence>(static_cast<int>(p) - i);
	}
	inline constexpr bool operator > (Precedence l, Precedence r)
	{
		return static_cast<int>(l) > static_cast<int>(r);
	}
}