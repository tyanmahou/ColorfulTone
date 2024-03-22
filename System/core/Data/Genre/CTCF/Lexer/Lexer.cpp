#include <core/Data/Genre/CTCF/Lexer/Lexer.hpp>

namespace ct::ctcf
{
    Lexer::Lexer(const FilePath& ctfolder)
    {
        this->load(ctfolder);
    }
	Lexer::Lexer(const s3d::Arg::code_<s3d::String>& script)
	{
		this->load(script);
	}
    bool Lexer::load(const FilePath& ctfolder)
	{
		TextReader reader(ctfolder);
		if (!reader) {
			return false;
		}
		String line;
		while (reader.readLine(line)) {
			if (line[0] == U'#') // オプション
			{
				this->pushOptions(line);
				continue;
			} else if (line[0] == U'%') // %はコメント
			{
				continue;
			}

			// パース
			this->parseLine(line);
		}

		return true;
	}
	bool Lexer::load(const s3d::Arg::code_<s3d::String>& script)
	{
		for (const String& line : script->split(U'\n')) {
			if (line[0] == U'#') // オプション
			{
				this->pushOptions(line);
				continue;
			} else if (line[0] == U'%') // %はコメント
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
		while (pos < line.length()) {
			// 空白スキップ
			while (s3d::IsSpace(line[pos])) {
				++pos;
			}
			if (s3d::IsAlpha(line[pos])) {
				// 識別子
				const size_t start = pos;
				while (pos < line.length() && (s3d::IsAlpha(line[pos]) || line[pos] == U'_')) {
					++pos;
				}
				m_tokens.emplace_back(line.substr(start, pos - start));
			} else if (::IsDigit(line[pos]) || line[pos] == U'-' && pos + 1 < line.length() && ::IsDigit(line[pos + 1])) {
				// 数
				const size_t start = pos;
				bool isFoundDot = false;

				++pos;
				while (pos < line.length()) {
					if (::IsDigit(line[pos])) {
						++pos;
					} else if (!isFoundDot && line[pos] == U'.' && pos + 1 < line.length() && ::IsDigit(line[pos + 1])) {
						isFoundDot = true;
						pos += 2;
					} else {
						break;
					}
				}
				m_tokens.emplace_back(line.substr(start, pos - start), TokenType::Number);
			} else if (line[pos] == U'"') {
				// 文字列

				++pos;
				const size_t start = pos;
				while (pos < line.length()) {
					if (line[pos] == U'\"' && line[pos - 1] != U'\\') {
						break;
					}
					++pos;
				}
				String strValue = line.substr(start, pos - start).replace(U"\\\"", U"\"");
				m_tokens.emplace_back(strValue, TokenType::String);
				++pos;
			} else if (line[pos] == U'★' || line[pos] == U'☆' || line[pos] == U'＊') {
				// ★
				const size_t start = pos;
				auto starChar = line[pos];
				++pos;
				while (pos < line.length()) {
					if (line[pos] != starChar) {
						break;
					}
					++pos;
				}
				m_tokens.emplace_back(line.substr(start, pos - start), TokenType::String);
			} else {
				// opなど
				if (line[pos] == U'!') {
					if (pos + 1 < line.length() && line[pos + 1] == U'=') {
						m_tokens.emplace_back(U"!=", TokenType::Op);
						pos += 2;
					} else {
						m_tokens.emplace_back(U"!", TokenType::Not);
						++pos;
					}
				} else if (line[pos] == U'=' && pos + 1 < line.length() && line[pos + 1] == U'=') {
					m_tokens.emplace_back(U"==", TokenType::Op);
					pos += 2;
				} else if (line[pos] == U'>') {
					if (pos + 1 < line.length() && line[pos + 1] == U'=') {
						m_tokens.emplace_back(U">=", TokenType::Op);
						pos += 2;
					} else {
						m_tokens.emplace_back(U">", TokenType::Op);
						++pos;
					}
				} else if (line[pos] == U'<') {
					if (pos + 1 < line.length() && line[pos + 1] == U'=') {
						m_tokens.emplace_back(U"<=", TokenType::Op);
						pos += 2;
					} else {
						m_tokens.emplace_back(U"<", TokenType::Op);
						++pos;
					}
				} else if (line[pos] == U'&' && pos + 1 < line.length() && line[pos + 1] == U'&') {
					m_tokens.emplace_back(U"&&", TokenType::And);
					pos += 2;
				} else if (line[pos] == U'|' && pos + 1 < line.length() && line[pos + 1] == U'|') {
					m_tokens.emplace_back(U"||", TokenType::Or);
					pos += 2;
				} else if (line[pos] == U'(' || line[pos] == U')') {
					m_tokens.emplace_back(String(1, line[pos]));
					++pos;
				} else {
					// そのほか
					const size_t start = pos;
					while (pos < line.length() && !s3d::IsSpace(line[pos])) {
						++pos;
					}
					m_tokens.emplace_back(line.substr(start, pos - start));
				}
			}
		}
	}
	void Lexer::pushOptions(const String& option)
	{
		auto parses = option.replaced(U" ", U"").split(U',');
		if (parses[0] == U"#TITLE" && parses.size() >= 2) {
			m_options[U"TITLE"] = parses[1];
		} else if (parses[0] == U"#ORDER" && parses.size() >= 2) {
			m_options[U"ORDER"] = parses[1];
		} else if (parses[0] == U"#EVAL" && parses.size() >= 2) {
			m_options[U"EVAL"] = parses[1];
		}
	}
}