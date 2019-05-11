#include "DownloadApi.hpp"
#include "ApiEndPoint.hpp"
#include "HttpRequest.hpp"

bool DownloadApi::List(s3d::Array<DownloadContent>& out)
{
	auto httpRequest = HttpRequest(Api::DownloadList);
	if (!httpRequest) {
		return false;
	}

	return true;
}
