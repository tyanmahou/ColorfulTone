#pragma once
#include <core/Data/Genre/CTCF/Lexer/Token.hpp>

namespace ct::ctcf
{
	enum class Precedence
	{
		Not,
		Comp,
		And,
		Or,
		Lowest,
	};

	inline Precedence ToPrecedence(TokenType token)
	{
		switch (token) {
		case TokenType::Not: return Precedence::Not;
		case TokenType::Op: return Precedence::Comp;
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
}