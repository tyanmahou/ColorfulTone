#pragma once
#include<Siv3D/JSONValue.hpp>

namespace JSON
{
	s3d::String Encode(const s3d::JSONValue& json);
	void Encode(const s3d::JSONValue& json, s3d::String& out);
}
