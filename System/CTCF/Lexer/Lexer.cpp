#include "Lexer.hpp"
#include <cwctype>

namespace
{
	bool IsWhiteSpace(wchar c)
	{
		return c == L' '
			|| c == L'\t'
			|| c == L'\r'
			|| c == L'\n'
			;
	}
	bool IsDigit(wchar c)
	{
		return L'0' <= c && c <= L'9';
	}
}
namespace ctcf
{
    Lexer::Lexer(const FilePath& ctfolder)
    {
        this->load(ctfolder);
    }
    bool Lexer::load(const FilePath& ctfolder)
	{
		TextReader reader(ctfolder);
		if (!reader) {
			return false;
		}
		String line;
		while (reader.readLine(line)) {
			if (line[0] == '#') // オプション
			{
				this->pushOptions(line);
				continue;
			} else if (line[0] == '%') // %はコメント
			{
				continue;
			}

			// パース
			this->parseLine(line);
		}

		return true;
	}
	Optional<String> Lexer::getOption(const String& option) const
	{
		if (m_options.find(option) != m_options.end()) {
			return m_options.at(option);
		}
		return s3d::none;
	}
	void Lexer::parseLine(const String& line)
	{
		size_t pos = 0;
		while (pos < line.length) {
			// 空白スキップ
			while (::IsWhiteSpace(line[pos])) {
				++pos;
			}
			if (std::iswalpha(line[pos])) {
				// 識別子
				const size_t start = pos;
				while (pos < line.length && (std::iswalnum(line[pos]) || line[pos] == L'_')) {
					++pos;
				}
				m_tokens.emplace_back(line.substr(start, pos - start));
			} else if (::IsDigit(line[pos]) || line[pos] == L'-' && pos + 1 < line.length && ::IsDigit(line[pos + 1])) {
				// 数
				const size_t start = pos;
				bool isFoundDot = false;

				++pos;
				while (pos < line.length) {
					if (::IsDigit(line[pos])) {
						++pos;
					} else if (!isFoundDot && line[pos] == L'.' && pos + 1 < line.length && ::IsDigit(line[pos + 1])) {
						isFoundDot = true;
						pos += 2;
					} else {
						break;
					}
				}
				m_tokens.emplace_back(line.substr(start, pos - start), TokenType::Number);
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
				m_tokens.emplace_back(strValue, TokenType::String);
				++pos;
			} else if (line[pos] == L'★') {
				// ★
				const size_t start = pos;
				++pos;
				while (pos < line.length) {
					if (line[pos] != L'★') {
						break;
					}
					++pos;
				}
				m_tokens.emplace_back(line.substr(start, pos - start), TokenType::String);
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
					m_tokens.emplace_back(String(1, line[pos]));
					++pos;
				} else {
					// そのほか
					const size_t start = pos;
					while (pos < line.length && !::IsWhiteSpace(line[pos])) {
						++pos;
					}
					m_tokens.emplace_back(line.substr(start, pos - start));
				}
			}
		}
	}
	void Lexer::pushOptions(const String& option)
	{
		auto parses = option.replace(L" ", L"").split(L',');
		if (parses[0] == L"#TITLE" && parses.size() >= 2) {
			m_options[L"TITLE"] = parses[1];
		} else if (parses[0] == L"#ORDER" && parses.size() >= 2) {
			m_options[L"ORDER"] = parses[1];
		} else if (parses[0] == L"#EVAL" && parses.size() >= 2) {
			m_options[L"EVAL"] = parses[1];
		}
	}
}