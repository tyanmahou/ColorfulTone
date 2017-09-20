#pragma once
#include"Game.h"

class GIFTexture 
{
	private:
		unsigned int m_frame;			//���t���[���Ŏ��̃C���[�W�ɂ���邩
		unsigned int m_frameCount;		//���݂̃J�E���g�l
		unsigned int m_nowImage;		//���݂̕`��t���[��
		Array<Image> m_images;
		DynamicTexture m_texture;

	public:
		GIFTexture(const FilePath& path, const unsigned  int frame=1);
		void update();
		void draw()const;

};