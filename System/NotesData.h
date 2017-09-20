#pragma once
#include"Object.h"
#include"Note.h"
#include"Bar.h"
#include"BPMChanger.h"
#include"LongNote.h"
#include"MahouSoundBeat.h"
#include"Stop.h"
#include"TextObject.h"
#include"Score.h"
class NotesData
{
public:
	static const int RESOLUTION = 9600;			//1���߂̃J�E���g��

private:
	int m_lv;									//���x��
	String m_lvName;							//���x����
	String m_notesArtistName;					//���ʐ���҂̖��O
	BPMType m_bpm;								//�ŏ���bpm

	double m_maxBarCount;						//���ߐ�
	int m_totalNotes;							//�m�[�c���̍��v
	int m_offsetSample;							//�I�t�Z�b�g�T���v����
	Array<std::shared_ptr<Object>> m_objects;	//���ʃf�[�^
	Array<StopRange> m_stopRanges;				//�X�g�b�v�̃f�[�^
	Array<TempoInfo> m_tempoInfos;				//�e���|�z��
	unsigned int m_currentBarIndex=0;			//���݂̃e���|

	String m_fileName;							//���ʃt�@�C���̖��O(�g���q���܂܂Ȃ�)

	bool m_isClear;								//�N���A���
	float m_clearRate;							//�B�������R�[�h
	SpecialResult m_specialResult;				//�t���R�����̏��

	Color m_color;								//�F

public:
	NotesData() = default;

	NotesData(const String& genreName, const String& dirPath, const String& filePath);

	void init();
	void synchroCount(const Sound& sound, double& nowCount);	//�J�E���g�Ɠ���

	void update(Sound& sound, double& nowCount, Score& score);
	void draw(const double& nowCount, float scrollRate)const;

	void previewDraw(const double & nowCount, float scrollRate) const;

	//�v���p�e�B�g���Ă݂����ǎg���K�v�Ȃ��Ǝv��
	__declspec(property(get = _getIsClear, put = _setIsClear))bool isClear;
	__declspec(property(get = _getSPResult, put = _setSPResult))SpecialResult specialResult;
	__declspec(property(get = _getRecord, put = _setRecord))float clearRate;

	const double calDrawCount(const double currentCount)const
	{
		double drawCount = currentCount;
		for (auto&& elm : m_stopRanges)
		{
			if (currentCount >= elm.m_judgeCount)
			{
				drawCount -= std::min(elm.m_rangeCount, currentCount - elm.m_judgeCount);
			}

		}
		return drawCount;
	}


	const int& getLevel()const { return m_lv; }
	const double& getMaxBarCount()const { return m_maxBarCount; }
	const int& getTotalNotes()const { return m_totalNotes; }
	const String& getLevelName()const { return m_lvName; }
	const String& getNotesArtistName()const { return  m_notesArtistName; }
	const String& getFileName()const { return m_fileName; }

	const  bool _getIsClear()const { return m_isClear; }
	void _setIsClear(bool newState) { m_isClear = newState; }
	const SpecialResult _getSPResult()const { return m_specialResult; }
	void _setSPResult(SpecialResult sp) { m_specialResult = sp; }
	const float _getRecord()const { return m_clearRate; }
	void _setRecord(float clearRate) { m_clearRate = clearRate; }

	BPMType getCurrentBPM()const
	{
		return m_tempoInfos[m_currentBarIndex].m_bar.getBPM();
	}

	const Color& getColor()const
	{
		return m_color;
	}
	void load(CSVReader& csv);
};
