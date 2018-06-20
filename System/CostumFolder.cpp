#include"CostumFolder.hpp"
#include"MusicData.h"
#include"GenreManager.h"
#include<stack>

namespace
{
	//比較演算の決定をして条件式とする
	template<class _T>
	std::function<bool(MusicData& music)> OperationSet(const String& op, const _T& (MusicData::*f)()const, const _T& value)
	{
		std::function<bool(MusicData& music)> func = [](MusicData& m)->bool {return true; };

		func = [value, f, op](MusicData& music)->bool {return CompareForOperator((music.*f)(), value, op); };

		return func;
	}

	//論理演算の結合
	void ChainSet(const String& chain, std::function<bool(MusicData& music)>&func, std::function<bool(MusicData& music)>&f)
	{
		if (chain == L"OR")
		{
			func = [func, f](MusicData& music)->bool {return func(music) || f(music); };
		}
		else
		{
			func = [func, f](MusicData& music)->bool {return func(music) && f(music); };
		}

	}

	template<class _T>
	void FunctionSet(CSVReader&csv, int index, const _T& (MusicData::*memfunc)()const, std::function<bool(MusicData& music)>&func, String& chain)
	{
		const _T value = csv.get<_T>(index, 2);
		const String op = csv.get<String>(index, 1);

		auto f = ::OperationSet(op, memfunc, value);

		::ChainSet(chain, func, f);

		chain = L"AND";

	}

	template<class _T>
	void FunctionSetForNotesData(CSVReader&csv, int index, bool (MusicData::*memfunc)(const _T&, const String&)const, std::function<bool(MusicData&)>&func, String& chain)
	{
		const _T value = csv.get<_T>(index, 2);
		const String op = csv.get<String>(index, 1);

		std::function<bool(MusicData&)>f = [value, memfunc, op](MusicData& music)->bool {return (music.*memfunc)(value, op); };

		::ChainSet(chain, func, f);

		chain = L"AND";

	}
}

void LoadCustomFolder()
{
	const auto ctcfFiles = FileSystem::DirectoryContents(L"CustomFolder");

	//ここからデータ読み込み
	for (const auto& path : ctcfFiles)
	{
		if (path.includes(L"ctfolder"))
		{

			CSVReader csv(path);

			if (!csv)
				continue;

			//タイトル
			String title = L"CustomFolder";

			std::function<bool(MusicData& music)> func = [](MusicData& music)->bool {return true; };


			auto rows = csv.rows;				//行数
			String head;						//1列目のデータを文字列で
			String chain = L"AND";				//AND or OR
			std::stack<std::pair<std::function<bool(MusicData& music)>, String>> stack;//()用

			for (unsigned int i = 0; i < rows; ++i)
			{

				head = csv.get<String>(i, 0);

				if (head.isEmpty)			//空行はスルー
				{
					continue;
				}

				if (head[0] == '%')//コメント
				{
					continue;
				}
				if (head == L"(")
				{
					stack.emplace(func, chain);
					func = [](MusicData& music)->bool {return true; };
					chain = L"AND";
				}
				if (head == L")")
				{
					auto& top = stack.top();
					ChainSet(top.second, func, top.first);
					chain = L"AND";
					stack.pop();
				}
				if (head == L"#TITLE")
				{
					title = csv.get<String>(i, 1);
				}

				if (head == L"AND" || head == L"OR")
				{
					chain = head;
				}
				if (head == L"BPM" || head == L"MINBPM")
				{
					::FunctionSet(csv, i, &(MusicData::getMinBPM), func, chain);
				}

				if (head == L"MAXBPM")
				{
					::FunctionSet(csv, i, &(MusicData::getMaxBPM), func, chain);
				}

				if (head == L"ARTIST")
				{
					::FunctionSet(csv, i, &(MusicData::getArtistName), func, chain);
				}
				if (head == L"MUSICNAME")
				{
					::FunctionSet(csv, i, &(MusicData::getMusicName), func, chain);
				}
				if (head == L"GENRE")
				{
					::FunctionSet(csv, i, &(MusicData::getGenreName), func, chain);
				}
				if (head == L"LEVEL")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchLevel), func, chain);
				}
				if (head == L"LEVELNAME")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchLevelName), func, chain);
				}
				if (head == L"NOTESARTIST")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchNotesArtistName), func, chain);
				}
				if (head == L"TOTALNOTE")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchTotalNotes), func, chain);
				}
				if (head == L"CLEARRATE")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchClearRate), func, chain);
				}
				if (head == L"CLEAR")
				{
					::FunctionSetForNotesData(csv, i, &(MusicData::serchClear), func, chain);
				}
				if (head == L"SPECIAL")
				{
					const String value = csv.get<String>(i, 2);
					const String op = csv.get<String>(i, 1);

					SpecialResult s;
					if (value == L"AP")
						s = SpecialResult::All_Perfect;
					if (value == L"FC")
						s = SpecialResult::Full_Combo;


					std::function<bool(MusicData& music)>f = [value, op, s](MusicData& music)->bool {return music.serchSpecialResult(s, op); };

					::ChainSet(chain, func, f);

					chain = L"AND";

				}
			}
			GenreManager::Add(GenreType::Custom, title, [func](MusicData& music)->bool {return !func(music); });
		}
	}
}
