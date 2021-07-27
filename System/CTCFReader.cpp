#include "CTCFReader.hpp"
#include<Siv3D.hpp>
#include <cwctype>
#include"MusicData.h"
#include "ResultRank.h"

namespace
{
	enum class Precedence
	{
		Value,
		Not,
		Op,
		And,
		Or,
		Max,
		Root = Max - 1,
		Heightest = Value,
		Lowest = Root,
	};

	Precedence ToPrecedence(TokenType token)
	{
		switch (token) {
		case TokenType::Value:
		case TokenType::Bool:
		case TokenType::Bracket:
		case TokenType::Identifier:
			return Precedence::Value;
		case TokenType::Not: return Precedence::Not;
		case TokenType::Op: return Precedence::Op;
		case TokenType::And: return Precedence::And;
		case TokenType::Or: return Precedence::Or;
		default:
			return Precedence::Lowest;
		}
	}
	constexpr Precedence operator - (Precedence p, int i)
	{
		return static_cast<Precedence>(static_cast<int>(p) - i);
	}
	namespace AST
	{
		// Rule<0> ::= VALUE | ( Rule<4> )
		// Rule<1> ::= OP Rule<0> 
		// Rule<2> ::= Rule<1> OP Rule<1>
		// Rule<3> ::= Rule<2> && Rule<2>
		// Rule<4> ::= Rule<3> || Rule<3>
		// パーサー
		class Parser
		{
			using AstItr = Array<Token>::iterator;
		private:
			Array<Token> m_tokens;
			std::shared_ptr<INode> m_root = nullptr;

			template<Precedence N>
			std::shared_ptr<INode> get(AstItr& it)
			{
				std::shared_ptr<INode> left = this->get<N - 1>(it);
				while (it != m_tokens.end() && ToPrecedence(it->type) == N)
				{
					std::shared_ptr<INode> op = std::make_shared<Op>(*it++);
					std::shared_ptr<INode> right = this->get<N - 1>(it);
					left = std::make_shared<Expr>(left, op, right);
				}
				return left;
			}
			std::shared_ptr<INode> root()
			{
				auto itr = m_tokens.begin();
				return this->get<Precedence::Root>(itr);
			}
		public:
			Parser(const Array<Token>& tokens) :
				m_tokens(tokens)
			{}
			bool expression(const NotesData& notes)
			{
				if (!m_root)
				{
					// 構文木の作成
					m_root = root();
				}
				if (!m_root)
				{
					return false;
				}
				return m_root->eval(notes).toBool();
			}
		};
		template<>
		std::shared_ptr<INode> Parser::get<Precedence::Value>(AstItr& it)
		{
			if (it->token == L"(")
			{
				auto node = this->get<Precedence::Root>(++it);
				if ((it++)->token != L")")
				{
					Println(L"ctfolder parse error: not found ')'");
				}
				return node;
			}
			if (it->type == TokenType::Bool) {
				return std::make_shared<BoolValue>(*it++);
			}
			return std::make_shared<Value>(*it++);
		}
		template<>
		std::shared_ptr<INode> Parser::get<Precedence::Not>(AstItr& it)
		{
			if (it != m_tokens.end() && it->type == TokenType::Not)
			{
				std::shared_ptr<INode> op = std::make_shared<Op>(*it++);
				std::shared_ptr<INode> right = this->get<Precedence::Not>(it);
				return std::make_shared<UnaryExpr>(op, right);
			}
			return this->get<Precedence::Not - 1>(it);
		}
	}
}

class CTCFReader::Impl
{
private:
	Lexer::CTCFLexer m_lexer;
	AST::Parser m_parser;
public:
	Impl(const s3d::FilePath & ctfolder) :
		m_lexer(ctfolder),
		m_parser(m_lexer.getTokens())
	{}

	s3d::Optional<s3d::String> getOption(const String & option)const
	{
		return m_lexer.getOption(option);
	}
	bool expression(const NotesData & notes)
	{
		return m_parser.expression(notes);
	}
	bool expression(const MusicData& music)
	{
		auto eval = this->getOption(L"EVAL").value_or(L"Any");
		if (eval == L"ALL") {
			if (music.getNotesData().empty()) {
				return false;
			}
			for (const auto& notes : music.getNotesData()) {
				if (!this->expression(notes)) {
					return false;
				}
			}
			return true;
		} else {
			for (const auto& notes : music.getNotesData()) {
				if (this->expression(notes)) {
					return true;
				}
			}
		}
		return false;
	}
	operator bool()const
	{
		return !m_lexer.getTokens().empty();
	}
};

CTCFReader::CTCFReader(const s3d::FilePath & ctfolder) :
	m_pImpl(std::make_shared<Impl>(ctfolder))
{}

CTCFReader::operator bool() const
{
	return m_pImpl->operator bool();
}

s3d::Optional<s3d::String> CTCFReader::getOption(const s3d::String & option) const
{
	return m_pImpl->getOption(option);
}

bool CTCFReader::expression(const MusicData & music) const
{
	return m_pImpl->expression(music);
}