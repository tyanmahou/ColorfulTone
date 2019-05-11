#include "DownloadApi.hpp"
#include "ApiEndPoint.hpp"
#include "HttpRequest.hpp"
#include "DownloadContent.hpp"

bool DownloadApi::List(s3d::Array<DownloadContent>& out)
{
	auto httpRequest = HttpRequest(Api::DownloadList);
	if (!httpRequest) {
		return false;
	}
	const auto & contents = httpRequest.getResponse().getArray();

	out.clear();
	out.reserve(contents.size());
	for (const auto& content : contents) {
		out.emplace_back(content);
	}
	return true;
}
