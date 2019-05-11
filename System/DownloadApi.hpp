#pragma once
#include<Siv3D/Array.hpp>

class DownloadContent;

class DownloadApi
{
public:
	static bool List(s3d::Array<DownloadContent>& out);
};