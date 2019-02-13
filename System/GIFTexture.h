#pragma once
#include<Siv3D/Fwd.hpp>
#include<memory>

class GIFTexture 
{
	private:
		class Impl;
		std::shared_ptr<Impl> m_pImpl;
	public:
		GIFTexture(const s3d::FilePath& path, const unsigned  int frame=1);
		void update()const;
		void draw()const;
};