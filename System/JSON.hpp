#pragma once
#include<Siv3D/JSONValue.hpp>

class JSON :public s3d::JSONValue
{
	struct null_t {};
public:
	static constexpr null_t null{};

	JSON(null_t) :
		JSONValue(ValueType::Null, true)
	{}

	JSON(bool b) :
		JSONValue(b)
	{}

	JSON(int number) :
		JSONValue(static_cast<double>(number))
	{}

	JSON(double number) :
		JSONValue(number)
	{}

	JSON(const s3d::String& str) :
		JSONValue(str)
	{}

	JSON(const s3d::JSONObject& object) :
		JSONValue(object)
	{}

	JSON(const s3d::JSONArray& ar) :
		JSONValue(ar)
	{}

	static s3d::JSONArray Array(const std::initializer_list<JSON>& values);

	static s3d::JSONObject Object(const std::unordered_map<s3d::String, JSON>& values);

	static s3d::String Encode(const s3d::JSONValue& json);
	static void Encode(const s3d::JSONValue& json, s3d::String& out);
};

