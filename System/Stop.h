#pragma once

struct StopRange
{
	double m_judgeCount;
	double m_rangeCount;
	StopRange(double judgeCount, double rangeCount) :
		m_judgeCount(judgeCount),
		m_rangeCount(rangeCount)
	{}
};

struct StopInfo
{
	double m_firstCount;//�ŏI�X�V�̂������X�g�b�v�̃J�E���g
	double m_totalOffset; //���܂ł̃g�[�^���̒�~���Ԃɂ���I�t�Z�b�g�Ԃ�
	StopInfo():
		m_firstCount(0),
		m_totalOffset(0)
	{}
	StopInfo(double firstCount,double totalOffset) :
		m_firstCount(firstCount),
		m_totalOffset(totalOffset)
	{}

};
