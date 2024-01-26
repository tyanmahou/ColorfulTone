#include <utils/Encode/EncodeUtil.hpp>
#include <fstream>
#include <filesystem>
#include <Siv3D.hpp>
#include <Siv3D/Windows/Windows.hpp>

namespace
{
    enum class CTEncode
    {
        Unknown,
        Unicode,
        ASCII,
        JIS,
        EUC,
        SJIS,
        UTF8,
    };
    using CTByte = unsigned char;

    /// <summary>
    /// エンコード取得
    /// https://dobon.net/vb/dotnet/string/detectcode.html#bom
    /// http://openlab.ring.gr.jp/Jcode/index-j.html
    /// </summary>
    /// <param name="txt"></param>
    /// <returns></returns>
    CTEncode GetEncode(const std::string& txt)
    {
        const CTByte* bytes = reinterpret_cast<const CTByte*>(txt.c_str());
        const CTByte bEscape = 0x1B;
        const CTByte bAt = 0x40;
        const CTByte bDollar = 0x24;
        const CTByte bAnd = 0x26;
        const CTByte bOpen = 0x28;    //'('
        const CTByte bB = 0x42;
        const CTByte bD = 0x44;
        const CTByte bJ = 0x4A;
        const CTByte bI = 0x49;

        size_t len = txt.size();
        CTByte b1, b2, b3, b4;

        //Encode::is_utf8 は無視

        bool isBinary = false;
        for (size_t i = 0; i < len; i++) {
            b1 = bytes[i];
            if (b1 <= 0x06 || b1 == 0x7F || b1 == 0xFF) {
                //'binary'
                isBinary = true;
                if (b1 == 0x00 && i < len - 1 && bytes[i + 1] <= 0x7F) {
                    //smells like raw unicode
                    return CTEncode::Unicode;
                }
            }
        }
        if (isBinary) {
            return CTEncode::Unknown;
        }

        //not Japanese
        bool notJapanese = true;
        for (size_t i = 0; i < len; i++) {
            b1 = bytes[i];
            if (b1 == bEscape || 0x80 <= b1) {
                notJapanese = false;
                break;
            }
        }
        if (notJapanese) {
            return CTEncode::ASCII;
        }

        for (size_t i = 0; i < len - 2; i++) {
            b1 = bytes[i];
            b2 = bytes[i + 1];
            b3 = bytes[i + 2];

            if (b1 == bEscape) {
                if (b2 == bDollar && b3 == bAt) {
                    //JIS_0208 1978
                    //JIS
                    return CTEncode::JIS;
                } else if (b2 == bDollar && b3 == bB) {
                    //JIS_0208 1983
                    //JIS
                    return CTEncode::JIS;
                } else if (b2 == bOpen && (b3 == bB || b3 == bJ)) {
                    //JIS_ASC
                    //JIS
                    return CTEncode::JIS;
                } else if (b2 == bOpen && b3 == bI) {
                    //JIS_KANA
                    //JIS
                    return CTEncode::JIS;
                }
                if (i < len - 3) {
                    b4 = bytes[i + 3];
                    if (b2 == bDollar && b3 == bOpen && b4 == bD) {
                        //JIS_0212
                        //JIS
                        return CTEncode::JIS;
                    }
                    if (i < len - 5 &&
                        b2 == bAnd && b3 == bAt && b4 == bEscape &&
                        bytes[i + 4] == bDollar && bytes[i + 5] == bB) {
                        //JIS_0208 1990
                        //JIS
                        return CTEncode::JIS;
                    }
                }
            }
        }
        int sjis = 0;
        int euc = 0;
        int utf8 = 0;
        for (size_t i = 0; i < len - 1; i++) {
            b1 = bytes[i];
            b2 = bytes[i + 1];
            if (((0x81 <= b1 && b1 <= 0x9F) || (0xE0 <= b1 && b1 <= 0xFC)) &&
                ((0x40 <= b2 && b2 <= 0x7E) || (0x80 <= b2 && b2 <= 0xFC))) {
                //SJIS_C
                sjis += 2;
                i++;
            }
        }
        for (size_t i = 0; i < len - 1; i++) {
            b1 = bytes[i];
            b2 = bytes[i + 1];
            if (((0xA1 <= b1 && b1 <= 0xFE) && (0xA1 <= b2 && b2 <= 0xFE)) ||
                (b1 == 0x8E && (0xA1 <= b2 && b2 <= 0xDF))) {
                //EUC_C
                //EUC_KANA
                euc += 2;
                i++;
            } else if (i < len - 2) {
                b3 = bytes[i + 2];
                if (b1 == 0x8F && (0xA1 <= b2 && b2 <= 0xFE) &&
                    (0xA1 <= b3 && b3 <= 0xFE)) {
                    //EUC_0212
                    euc += 3;
                    i += 2;
                }
            }
        }
        for (size_t i = 0; i < len - 1; i++) {
            b1 = bytes[i];
            b2 = bytes[i + 1];
            if ((0xC0 <= b1 && b1 <= 0xDF) && (0x80 <= b2 && b2 <= 0xBF)) {
                //UTF8
                utf8 += 2;
                i++;
            } else if (i < len - 2) {
                b3 = bytes[i + 2];
                if ((0xE0 <= b1 && b1 <= 0xEF) && (0x80 <= b2 && b2 <= 0xBF) &&
                    (0x80 <= b3 && b3 <= 0xBF)) {
                    //UTF8
                    utf8 += 3;
                    i += 2;
                }
            }
        }
        if (euc > sjis && euc > utf8) {
            //EUC
            return CTEncode::EUC;
        } else if (sjis > euc && sjis > utf8) {
            //SJIS
            return CTEncode::SJIS;
        } else if (utf8 > euc && utf8 > sjis) {
            //UTF8
            return CTEncode::UTF8;
        }

        return CTEncode::Unknown;
    }

    bool TryGetShiftJISText(s3d::FilePathView filePath, std::string& out)
    {
        if (s3d::TextEncoding encode = Unicode::GetTextEncoding(filePath); encode != TextEncoding::UTF8_NO_BOM) {
            // UTF-8 NO BOMじゃなければそもそも違う
            return false;
        }
        std::ifstream file(filePath.narrow(), std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        out = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        CTEncode encode = ::GetEncode(out);
        return encode == CTEncode::SJIS || encode == CTEncode::ASCII;
    }
    // Shift-JISからUTF-8への変換関数
    std::string ConvertShiftJISToUTF8(const std::string& sjisStr) {
        // Shift-JISからUTF-16に変換
        int utf16Size = ::MultiByteToWideChar(CP_ACP, 0, sjisStr.c_str(), -1, NULL, 0);
        std::wstring utf16Str(utf16Size, L'\0');
        ::MultiByteToWideChar(CP_ACP, 0, sjisStr.c_str(), -1, &utf16Str[0], utf16Size);

        // UTF-16からUTF-8に変換
        int utf8Size = ::WideCharToMultiByte(CP_UTF8, 0, utf16Str.c_str(), -1, NULL, 0, NULL, NULL);
        std::string utf8Str(utf8Size, '\0');
        WideCharToMultiByte(CP_UTF8, 0, utf16Str.c_str(), -1, &utf8Str[0], utf8Size, NULL, NULL);

        return utf8Str;
    }
}
namespace ct
{
    bool EncodeUtil::IsMaybeShiftJIS(s3d::FilePathView filePath)
    {
        [[maybe_unused]] std::string _;
        return ::TryGetShiftJISText(filePath, _);
    }
    bool EncodeUtil::ConvertShiftJISToUTF8(s3d::FilePathView filePath)
    {
        std::string text;
        if (!::TryGetShiftJISText(filePath, text)) {
            // ShiftJISじゃない
            return false;
        }
        std::string utf8Content = ::ConvertShiftJISToUTF8(text);

        // UTF-8でファイルを再度書き込む
        {
            TextWriter writer(filePath);
            if (!writer) {
                return false;
            }
            writer.writeUTF8(utf8Content);
        }
        return true;
    }
}
