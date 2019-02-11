#include "CTCFReader.h"
#include<Siv3D.hpp>

#include"MusicData.h"

// token
namespace
{
	enum class TokenType
	{
		Id,
		Operater,
		Value,
	};
	struct Token
	{
		String token;
		TokenType type;

		Token(const String& str)
		{

		}
	};
}
// AST
namespace
{
#define RETURN_COMPARE(OP) \
	if(op == L""#OP) { \
		return a OP b; \
	}

	template<class _T>
	bool Compare(const _T& a, const _T& b, const String& op)
	{
		RETURN_COMPARE(<);
		RETURN_COMPARE(<=);
		RETURN_COMPARE(>);
		RETURN_COMPARE(>=);
		RETURN_COMPARE(!=);
		RETURN_COMPARE(==);
		return false;
	}

	template<>
	bool Compare(const String& a, const String& b, const String& op)
	{
		RETURN_COMPARE(<);
		RETURN_COMPARE(<=);
		RETURN_COMPARE(>);
		RETURN_COMPARE(>=);
		RETURN_COMPARE(!=);
		RETURN_COMPARE(==);
		if (op == L"includes")
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

	namespace AST
	{
		class Result
		{
		private:
			union 
			{
				String str;
				bool b;
			};
		public:
			Result(const String& value):
				str(value)
			{}
			Result(bool value) :
				b(value)
			{}
			~Result()
			{};
			bool toBool()const
			{
				return this->b;
			}
			const String& toString()const
			{
				return this->str;
			}
		};
		class INode
		{
		public:
			virtual Result eval(const MusicData&) = 0;
		};
		class ILeaf : public INode
		{
		protected:
			std::shared_ptr<Token> m_token;
		public:
			ILeaf(std::shared_ptr<Token> token) :
				m_token(token)
			{}
		};
		class Value : public ILeaf
		{
		public:
			Value(std::shared_ptr<Token> token) :
				ILeaf(token)
			{}
			Result eval(const MusicData&)override
			{
				return { m_token->token };
			}
		};
		using Id = Value;

		class Operator : public INode
		{
		private:
			std::shared_ptr<INode> m_left;
			std::shared_ptr<INode> m_op;
			std::shared_ptr<INode> m_right;
			Operator(std::shared_ptr<INode> left, std::shared_ptr<INode> op, std::shared_ptr<INode> right) :
				m_left(left),
				m_op(op),
				m_right(right)
			{}

			bool compare(const MusicData& music, const String& id, const String& op, const String& value)
			{
				if (id == L"BPM" || id == L"MINBPM")
				{
					return ::Compare(music.getMinBPM(), Parse<double>(value), op);
				}
				if (id == L"MAXBPM")
				{
					return ::Compare(music.getMaxBPM(), Parse<double>(value), op);
				}
				if (id == L"ARTIST")
				{
					return ::Compare(music.getArtistName(),value, op);
				}
				if (id == L"AUTHORITY")
				{
					const auto& authority = music.getAuthority();
					if (authority.has_value()) 
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
						if (::Compare(elm.getLevel(),level, op))
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
				if (id == L"NOTESARTIST")
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
						if (::Compare(elm.clearRate, rate, op))
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
						if (::Compare(elm.isClear, isClear, op))
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
						if (::Compare(elm.specialResult, s, op))
						{
							return true;
						}
					}
					return false;
				}
				return false;
			}
		public:
			Result eval(const MusicData& music) override
			{
				const String& op = m_op->eval(music).toString();
				if (op == L"&&")
				{
					return m_right->eval(music).toBool() && m_left->eval(music).toBool();
				}
				if (op == L"||")
				{
					return m_right->eval(music).toBool() || m_left->eval(music).toBool();
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
	}
}
namespace
{
	using Options = std::unordered_map<String, String>;

	class CTCFLexer
	{
		Options m_options;
		Array<Token> m_tokens;
		void pushOptions(const String& option)
		{
			auto& parses = option.replace(L" ", L"").split(L',');
			if (parses[0] == L"#TITLE" && parses.size() >= 2)
			{
				m_options[L"TITLE"] = parses[1];
			}
		}

	public:
		CTCFLexer(const FilePath & ctcfPath)
		{
		}
		bool load(const FilePath & ctcfPath)
		{
			TextReader reader(ctcfPath);
			if (!reader)
			{
				return false;
			}
			String line;
			String all;
			while (reader.readLine(line))
			{
				if (line[0] == '#') // オプション
				{
					this->pushOptions(line);
				}
				else if (line[0] != '%') // %はコメント
				{
					all += line + L" ";
				}
			}
			for (auto&& str : all.split(L' '))
			{
				if (!str.isEmpty) 
				{
					m_tokens.emplace_back(str);
				}
			}
			return true;
		}
	};
}

CTCFReader::CTCFReader(const s3d::FilePath & ctcfPath)
{
}
