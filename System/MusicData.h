#pragma once
#include"NotesData.h"


template<class _T>
bool CompareForOperator(const _T& a, const _T& b, const String& op)
{
	if (op == L"=")
	{
		return a == b;
	}
	else if (op == L"<")
	{
		return a < b;
	}
	else if (op == L"<=")
	{
		return a <= b;
	}
	else 	if (op == L">")
	{
		return a > b;
	}
	else if (op == L">=")
	{
		return a >= b;
	}
	else if (op == L"!=")
	{
		return a != b;
	}

	return false;
}

template<>
inline bool CompareForOperator(const String& a, const String& b, const String& op)
{
	if (op == L"=")
	{
		return a == b;
	}
	else if (op == L"<")
	{
		return a < b;
	}
	else if (op == L"<=")
	{
		return a <= b;
	}
	else 	if (op == L">")
	{
		return a > b;
	}
	else if (op == L">=")
	{
		return a >= b;
	}
	else if (op == L"!=")
	{
		return a != b;
	}
	else if (op == L"includes")
	{
		return a.includes(b);
	}
	return false;
}


class MusicData 
{
public:
	using ABLoop = Vec2;

	static int Index;
private:

	String m_musicName;		//�Ȗ�
	String m_artistName;	//�A�[�e�B�X�g��
	String m_genreName;		//�W��������
	Texture m_texture;		//�W���P�G

	BPMType m_minbpm,m_maxbpm;	//bpm

	Mahou::SoundBar m_minBar, m_maxBar;//bpm��beat

	mutable EasingController<BPMType> m_bpm;	//�Ԃ�l�p

	ABLoop m_loop;					//���[�v�͈�

	Array<NotesData> m_notesDatas;	//���ʏ��

	String m_fileName;		//�t�@�C���̖��O

	String m_soundNameID;	//�A�Z�b�g�ւ̃A�N�Z�X

	int m_index;

public:
		MusicData() = default;
		MusicData(const String& genreName,const String& dirPath, const String& iniPath);

		 const Texture getTexture()const{ return m_texture;}

		  Sound getSound(){ return SoundAsset(m_soundNameID); }
		const String& getMusicName()const { return m_musicName; }
		const String& getArtistName()const { return m_artistName; }
		const String& getFileName()const { return m_fileName; }
		const String& getGenreName()const { return m_genreName; }
		const ABLoop& getLoopRange()const { return m_loop; }
		const int getBPM()const
		{
			if (m_maxbpm == -1)return m_minbpm;
			if (!m_bpm.isActive()) 
			{
				static int tmp = 240;
				if (tmp == 0)
					tmp = 240;
				if(tmp>0)
				tmp--;
				if(tmp==0)
				m_bpm.start();
			}
			return (int)m_bpm.easeInOut();
		}
		bool includesLV(const int& lv) const
		{
			return serchLevel(lv);
		}
		bool serchClear(const bool& isClear, const String& op = L"=")const
		{
			for (const auto& elm : m_notesDatas)
			{
				if (CompareForOperator(elm.isClear, isClear, op))
				{
					return true;
				}
			}
			return false;
		}
		bool serchSpecialResult(const SpecialResult& sr, const String& op = L"=")const
		{
			for (const auto& elm : m_notesDatas)
			{
				if (op==L"="&&sr == elm.specialResult)
					return true;
				else
					if (op == L"!="&&sr != elm.specialResult)
						return true;

			}
			return false;
		}

		bool serchClearRate(const float& clearRate,const String& op=L"=")const
		{
			for (const auto& elm : m_notesDatas)
			{
				if (CompareForOperator(elm.clearRate, clearRate, op))
				{
					return true;
				}
			}
			return false;
		}
		bool serchLevel(const int& level, const String& op = L"=")const
		{
			for (const auto& elm : m_notesDatas)
			{
				if (CompareForOperator(elm.getLevel(), level, op))
				{
					return true;
				}
			}
			return false;
		}
		bool serchTotalNotes(const int& totalNotes, const String& op = L"=")const
		{
			for (const auto& elm : m_notesDatas)
			{
				if (CompareForOperator(elm.getTotalNotes(), totalNotes, op))
				{
					return true;
				}
			}
			return false;
		}
		bool serchNotesArtistName(const String&artistName, const String& op = L"=")const
		{
			for (const auto& elm : m_notesDatas)
			{
				if (CompareForOperator(elm.getNotesArtistName(), artistName, op))
				{
					return true;
				}
			}
			return false;
		}
		bool serchLevelName(const String&levelName, const String& op = L"=")const
		{
			for (const auto& elm : m_notesDatas)
			{
				if (CompareForOperator(elm.getLevelName(), levelName, op))
				{
					return true;
				}
			}
			return false;
		}


		const Mahou::SoundBar& getMinSoundBeat()const { return m_minBar; }
		const Mahou::SoundBar& getMaxSoundBeat()const { if (m_maxbpm == -1)return m_minBar; return m_maxBar; }
		const BPMType& getMinBPM()const { return m_minbpm; };
		const BPMType& getMaxBPM()const { if (m_maxbpm == -1)return m_minbpm; return m_maxbpm;};

	    Array<NotesData>& getNotesData(){ return m_notesDatas; }

		const String& getSoundNameID()const { return m_soundNameID; }
		const int getIndex()const { return m_index; }


};
