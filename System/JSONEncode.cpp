#include "JSONEncode.hpp"
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

namespace JSON
{
	String Encode(const JSONValue& json)
	{
		String out;
		Encode(json, out);
		return out;
	}

	void Encode(const JSONValue& json, String& out)
	{
		::Dump(json, out);
	}
}
