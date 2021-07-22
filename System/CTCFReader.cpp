#include "CTCFReader.hpp"
#include<Siv3D.hpp>
#include"MusicData.h"

namespace
{
	//----------------------------------------------------------
	// Token
	//----------------------------------------------------------

	enum class TokenType : size_t
	{
		Value = 0, Bracket = Value,
		Not,
		Op,
		And,
		Or,
		MAX,
	};

	constexpr size_t Root = static_cast<size_t>(TokenType::MAX) - 1u;

	TokenType GetTokenType(const String& str)
	{
		static const std::unordered_map<String, TokenType> ids
		{
			{L"(", TokenType::Bracket},
			{L")", TokenType::Bracket},
			{L"!", TokenType::Not},
			{L"==", TokenType::Op},
			{L"!=", TokenType::Op},
			{L"<", TokenType::Op},
			{L"<=", TokenType::Op},
			{L">", TokenType::Op},
			{L">=", TokenType::Op},
			{L"includes", TokenType::Op},
			{L"in", TokenType::Op},
			{L"&&", TokenType::And},
			{L"||", TokenType::Or},
		};
		if (ids.find(str) != ids.end()) {
			return ids.at(str);
		}
		return TokenType::Value;
	}
	struct Token
	{
		String token;
		TokenType type;

		Token(const String& str):
			token(str),
			type(::GetTokenType(str))
		{}
		Token(const String& str, TokenType _type) :
			token(str),
			type(_type)
		{}
	};
	//----------------------------------------------------------
	// AST
	//----------------------------------------------------------
#define RETURN_COMPARE(OP) \
	if(op == L""#OP) { \
		return a OP b; \
	}

	template<class _T>
	bool Compare(const _T& a, const _T& b, const String& op)
	{
		RETURN_COMPARE(== );
		RETURN_COMPARE(!= );
		RETURN_COMPARE(< );
		RETURN_COMPARE(<= );
		RETURN_COMPARE(> );
		RETURN_COMPARE(>= );
		return false;
	}

	template<>
	bool Compare(const String& a, const String& b, const String& op)
	{
		RETURN_COMPARE(== );
		RETURN_COMPARE(!= );
		RETURN_COMPARE(< );
		RETURN_COMPARE(<= );
		RETURN_COMPARE(> );
		RETURN_COMPARE(>= );
		if (op == L"includes" || op == L"in")
		{
			return a.includes(b);
		}
		return false;
	}
	template<>
	bool Compare(const SpecialResult& a, const SpecialResult& b, const String& op)
	{
		RETURN_COMPARE(!= );
		RETURN_COMPARE(== );
		return false;
	}
#undef RETURN_COMPARE

	class Result
	{
	public:
		enum class ValueType
		{
			Null,
			String,
			Bool,
		};

	private:
		union
		{
			s3d::String* str;
			bool b;
		}m_value;

		ValueType m_type;
	public:
		Result() :
			m_type(ValueType::Null)
		{
			memset(&m_value, 0, sizeof(m_value));
		}

		Result(const Result& other) :
			m_type(ValueType::Null)
		{
			*this = other;
		}
		Result(Result&& other) noexcept :
			m_type(ValueType::Null)
		{
			*this = std::move(other);
		}

		Result(const String& value) :
			m_type(ValueType::String)
		{
			m_value.str = new s3d::String(value);
		}
		Result(bool value) :
			m_type(ValueType::Bool)
		{
			m_value.b = value;
		}
		~Result()
		{
			this->clear();
		}
		bool toBool()const
		{
			switch (m_type) {
			case ValueType::Bool:
				return this->m_value.b;
			default:
				return false;
			}
		}
		String toString()const
		{
			switch (m_type) {
			case ValueType::String:
				return *this->m_value.str;
			default:
				return L"";
			}
		}

		void clear()
		{
			switch (this->m_type) {
			case ValueType::String:
				delete this->m_value.str;
				this->m_value.str = nullptr;
				break;
			case ValueType::Bool:
				this->m_value.b = false;
				break;
			default:
				break;
			}
			this->m_type = ValueType::Null;
		}
		void reset(ValueType type)
		{
			if (m_type == type) {
				return;
			}
			this->clear();
			switch (type) {
			case ValueType::String:
				m_value.str = new (std::nothrow) s3d::String();
				break;
			default:
				break;
			}
			m_type = type;
		}
		Result& operator=(const Result& other)
		{
			if (this == &other) {
				return *this;
			}
			this->reset(other.m_type);

			switch (other.m_type) {
			case ValueType::String:
				if (this->m_value.str == nullptr) {
					this->m_value.str = new s3d::String();
				}
				*this->m_value.str = *other.m_value.str;
				break;
			case ValueType::Bool:
				this->m_value.b = other.m_value.b;
				break;
			default:
				break;
			}
			return *this;
		}
		Result& operator=(Result&& other) noexcept
		{
			if (this == &other) {
				return *this;
			}
			this->clear();
			switch (other.m_type) {
			case ValueType::String:
				this->m_value.str = other.m_value.str;
				break;
			case ValueType::Bool:
				this->m_value.b = other.m_value.b;
				break;
			default:
				break;
			}
			this->m_type = other.m_type;
			other.m_type = ValueType::Null;
			memset(&other.m_value, 0, sizeof(other.m_value));

			return *this;
		}
	};
	namespace AST
	{

		class INode
		{
		public:
			virtual Result eval(const MusicData&) = 0;
		};
		class ILeaf : public INode
		{
		protected:
			Token m_token;
		public:
			ILeaf(const Token& token) :
				m_token(token)
			{}
		};
		class Value : public ILeaf
		{
		public:
			Value(const Token& token) :
				ILeaf(token)
			{}
			Result eval(const MusicData&)override
			{
				return { m_token.token };
			}
		};
		using Op = Value;

		class UnaryExpr : public INode
		{
		private:
			std::shared_ptr<INode> m_op;
			std::shared_ptr<INode> m_right;
		public:
			UnaryExpr(std::shared_ptr<INode> op, std::shared_ptr<INode> right) :
				m_op(op),
				m_right(right)
			{}
			Result eval(const MusicData& music) override
			{
				const String& op = m_op->eval(music).toString();
				if (op == L"!")
				{
					return !m_right->eval(music).toBool();
				}
				return m_right->eval(music);
			}
		};

		class Expr : public INode
		{
		private:
			std::shared_ptr<INode> m_left;
			std::shared_ptr<INode> m_op;
			std::shared_ptr<INode> m_right;

			bool compare(const MusicData& music, const String& id, const String& op, const String& value)
			{
				if (id == L"BPM")
				{
					auto bpm = Parse<double>(value);
					return ::Compare(music.getMinBPM(), bpm, op) || ::Compare(music.getMaxBPM(), bpm, op);
				}
				if (id == L"MINBPM") {
					return ::Compare(music.getMinBPM(), Parse<double>(value), op);
				}
				if (id == L"MAXBPM")
				{
					return ::Compare(music.getMaxBPM(), Parse<double>(value), op);
				}
				if (id == L"ARTIST")
				{
					return ::Compare(music.getArtistName(), value, op);
				}
				if (id == L"AUTHORITY")
				{
					const auto& authority = music.getAuthority();
					if (authority.has_value() && !authority.value().isEmpty)
					{
						return ::Compare(authority.value(), value, op);
					}
					return false;
				}
				if (id == L"MUSICNAME")
				{
					return ::Compare(music.getMusicName(), value, op);
				}
				if (id == L"GENRE")
				{
					return ::Compare(music.getGenreName(), value, op);
				}
				if (id == L"LEVEL")
				{
					int level = Parse<int>(value);
					for (const auto& elm : music.getNotesData())
					{
						if (::Compare(elm.getLevel(), level, op))
						{
							return true;
						}
					}
					return false;
				}
				if (id == L"LEVELNAME")
				{
					for (const auto& elm : music.getNotesData())
					{
						if (::Compare(elm.getLevelName(), value, op))
						{
							return true;
						}
					}
					return false;
				}
				if (id == L"STAR") {
					for (const auto& elm : music.getNotesData()) {
						if (::Compare(ToStr(elm.getStarLv()), value, op)) {
							return true;
						}
					}
					return false;
				}
				if (id == L"NOTE" || id == L"NOTESARTIST" || id == L"NOTESDESIGNER")
				{
					for (const auto& elm : music.getNotesData())
					{
						if (::Compare(elm.getNotesArtistName(), value, op))
						{
							return true;
						}
					}
					return false;
				}
				if (id == L"TOTALNOTE")
				{
					int total = Parse<int>(value);
					for (const auto& elm : music.getNotesData())
					{
						if (::Compare(elm.getTotalNotes(), total, op))
						{
							return true;
						}
					}
					return false;
				}
				if (id == L"CLEARRATE")
				{
					float rate = Parse<float>(value);
					for (const auto& elm : music.getNotesData())
					{
						if (::Compare(elm.getScore().clearRate, rate, op))
						{
							return true;
						}
					}
					return false;
				}
				if (id == L"CLEAR")
				{
					bool isClear = Parse<bool>(value);
					for (const auto& elm : music.getNotesData())
					{
						if (::Compare(elm.getScore().isClear, isClear, op))
						{
							return true;
						}
					}
					return false;
				}
				if (id == L"SPECIAL")
				{
					SpecialResult s;
					if (value == L"AP")
					{
						s = SpecialResult::All_Perfect;
					}
					else if (value == L"FC")
					{
						s = SpecialResult::Full_Combo;
					}
					for (const auto& elm : music.getNotesData())
					{
						if (::Compare(elm.getScore().specialResult, s, op))
						{
							return true;
						}
					}
					return false;
				}
				if (id == L"FAVORITE") {
					return ::Compare(music.isFavorite(), Parse<bool>(value), op);
				}
				return false;
			}
		public:
			Expr(std::shared_ptr<INode> left, std::shared_ptr<INode> op, std::shared_ptr<INode> right) :
				m_left(left),
				m_op(op),
				m_right(right)
			{}
			Result eval(const MusicData& music) override
			{
				const String& op = m_op->eval(music).toString();
				if (op == L"&&")
				{
					return m_left->eval(music).toBool() && m_right->eval(music).toBool();
				}
				if (op == L"||")
				{
					return m_left->eval(music).toBool() || m_right->eval(music).toBool();
				}
				return {
					this->compare(
					music,
					m_left->eval(music).toString(),
					op,
					m_right->eval(music).toString()
					)
				};
			}
		};


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

			template<size_t N>
			std::shared_ptr<INode> get(AstItr& it)
			{
				std::shared_ptr<INode> left = this->get<N - 1>(it);
				while (it != m_tokens.end() && static_cast<size_t>(it->type) == N)
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
				return this->get<Root>(itr);
			}
		public:
			Parser(const Array<Token>& tokens) :
				m_tokens(tokens)
			{}
			bool expression(const MusicData& music)
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
				return m_root->eval(music).toBool();
			}
		};
		template<>
		std::shared_ptr<INode> Parser::get<0>(AstItr& it)
		{
			if (it->token == L"(")
			{
				auto node = this->get<Root>(++it);
				if ((it++)->token != L")")
				{
					Println(L"ctfolder parse error: not found ')'");
				}
				return node;
			}
			return std::make_shared<Value>(*it++);
		}
		template<>
		std::shared_ptr<INode> Parser::get<1>(AstItr& it)
		{
			if (it != m_tokens.end() && it->type == TokenType::Not)
			{
				std::shared_ptr<INode> op = std::make_shared<Op>(*it++);
				std::shared_ptr<INode> right = this->get<1>(it);
				return std::make_shared<UnaryExpr>(op, right);
			}
			return this->get<0>(it);
		}
	}

	//----------------------------------------------------------
	// Lexer
	//----------------------------------------------------------
	namespace Lexer
	{
		using Options = std::unordered_map<String, String>;

		class CTCFLexer
		{
		public:
			CTCFLexer(const FilePath & ctfolder)
			{
				this->load(ctfolder);
			}
			bool load(const FilePath & ctfolder)
			{
				TextReader reader(ctfolder);
				if (!reader)
				{
					return false;
				}
				String line;
				while (reader.readLine(line))
				{
					if (line[0] == '#') // オプション
					{
						this->pushOptions(line);
						continue;
					}
					else if (line[0] == '%') // %はコメント
					{
						continue;
					}

					// パース
					this->parseLine(line);
				}

				return true;
			}
			const Array<Token>& getTokens()const
			{
				return m_tokens;
			}
			Optional<String> getOption(const String & option)const
			{
				if (m_options.find(option) != m_options.end())
				{
					return m_options.at(option);
				}
				return s3d::none;
			}
		private:
			void parseLine(const String& line)
			{
				size_t pos = 0;
				while (pos < line.length) {
					// 空白スキップ
					while (this->isWhiteSpace(line[pos])) {
						++pos;
					}
					if (this->isDigit(line[pos]) || line[pos] == L'-' && pos + 1< line.length && this->isDigit(line[pos + 1])) {
						// 数
						const size_t start = pos;
						bool isFoundDot = false;

						++pos;
						while (pos < line.length) {
							if (this->isDigit(line[pos])) {
								++pos;
							} else if(!isFoundDot && line[pos] == L'.' && pos + 1 < line.length && this->isDigit(line[pos + 1])){
								isFoundDot = true;
								pos += 2;
							} else {
								break;
							}
						}
						m_tokens.emplace_back(line.substr(start, pos - start), TokenType::Value);
					} else if (line[pos] == L'"') {
						// 文字列

						++pos;
						const size_t start = pos;
						while (pos < line.length) {
							if (line[pos] == L'\"' && line[pos - 1] != L'\\') {
								break;
							}
							++pos;
						}
						auto strValue = line.substr(start, pos - start).replace(L"\\\"", L"\"");
						m_tokens.emplace_back(strValue, TokenType::Value);
						++pos;
					} else {
						// opなど
						if (line[pos] == L'!') {
							if (pos + 1 < line.length && line[pos + 1] == L'=') {
								m_tokens.emplace_back(L"!=", TokenType::Op);
								pos += 2;
							} else {
								m_tokens.emplace_back(L"!", TokenType::Not);
								++pos;
							}
						} else if (line[pos] == L'=' && pos + 1 < line.length && line[pos + 1] == L'=') {
							m_tokens.emplace_back(L"==", TokenType::Op);
							pos += 2;
						} else if (line[pos] == L'>') {
							if (pos + 1 < line.length && line[pos + 1] == L'=') {
								m_tokens.emplace_back(L">=", TokenType::Op);
								pos += 2;
							} else {
								m_tokens.emplace_back(L">", TokenType::Op);
								++pos;
							}
						} else if (line[pos] == L'<') {
							if (pos + 1 < line.length && line[pos + 1] == L'=') {
								m_tokens.emplace_back(L"<=", TokenType::Op);
								pos += 2;
							} else {
								m_tokens.emplace_back(L"<", TokenType::Op);
								++pos;
							}
						} else if (line[pos] == L'&' && pos + 1 < line.length && line[pos + 1] == L'&') {
							m_tokens.emplace_back(L"&&", TokenType::And);
							pos += 2;
						} else if (line[pos] == L'|' && pos + 1 < line.length && line[pos + 1] == L'|') {
							m_tokens.emplace_back(L"||", TokenType::Or);
							pos += 2;
						} else if (line[pos] == L'(' || line[pos] == L')') {
							m_tokens.emplace_back(String(1, line[pos]), TokenType::Bracket);
							++pos;
						} else {
							// 識別子
							const size_t start = pos;
							while (pos < line.length && !this->isWhiteSpace(line[pos])) {
								++pos;
							}
							m_tokens.emplace_back(line.substr(start, pos - start));
						}
					}
				}
			}
			bool isWhiteSpace(wchar c)
			{
				return c == L' '
					|| c == L'\t'
					|| c == L'\r'
					|| c == L'\n'
					;
			}
			bool isDigit(wchar c)
			{
				return L'0' <= c && c <= L'9';
			}
			void pushOptions(const String& option)
			{
				auto parses = option.replace(L" ", L"").split(L',');
				if (parses[0] == L"#TITLE" && parses.size() >= 2) {
					m_options[L"TITLE"] = parses[1];
				} else if (parses[0] == L"#ORDER" && parses.size() >= 2) {
					m_options[L"ORDER"] = parses[1];
				}
			}
		private:
			Options m_options;
			Array<Token> m_tokens;
		};
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
	bool expression(const MusicData & music)
	{
		return m_parser.expression(music);
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