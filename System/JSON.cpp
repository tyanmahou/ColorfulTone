#include "JSON.hpp"
using namespace s3d;

namespace
{
	void Dump(const JSONValue& json, String& out);

	// json array
	void Dump(const JSONArray& json, String& out)
	{
		out += L"[";
		bool first = true;
		for (const auto& value : json) {
			if (!first) {
				out += L", ";
			}
			first = false;
			Dump(value, out);
		}
		out += L"]";
	}

	//json object
	void Dump(const JSONObject& json, String& out)
	{
		out += L"{";
		bool first = true;
		for (const auto& value : json) {
			if (!first) {
				out += L", ";
			}
			first = false;
			out += L'"' + value.first + L'"' + L": ";
			Dump(value.second, out);
		}
		out += L"}";
	}

	// json
	void Dump(const JSONValue& json, String& out)
	{
		if (json.isNull()) {
			out += L"null";
		}
		else if (json.isBool() || json.isNumber()) {
			out += json.to_str();
		}
		else if (json.isString()) {
			out += L'"' + json.getString() + L'"';
		}
		else if (json.isArray()) {
			Dump(json.getArray(), out);
		}
		else if (json.isObject()) {
			Dump(json.getObject(), out);
		}
	}
}

s3d::JSONArray JSON::Array(const std::initializer_list<JSON>& values)
{
	s3d::JSONArray ret;
	ret.reserve(values.size());
	for (auto&& value : values) {
		ret.push_back(value);
	}
	return ret;
}

s3d::JSONObject JSON::Object(const std::unordered_map<s3d::String, JSON>& values)
{
	s3d::JSONObject ret;
	ret.reserve(values.size());
	for (auto&& value : values) {
		ret[value.first] = value.second;
	}
	return ret;
}

String JSON::Encode(const JSONValue& json)
{
	String out;
	Encode(json, out);
	return out;
}

void JSON::Encode(const JSONValue& json, String& out)
{
	::Dump(json, out);
}
