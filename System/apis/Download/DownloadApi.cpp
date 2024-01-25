#include <apis/Download/DownloadApi.hpp>
#include <apis/ApiEndPoints.hpp>
#include <core/Data/DownloadContent/DownloadContent.hpp>
#include <utils/Http/RestRequestor.hpp>

namespace ct
{
	Coro::Fiber<s3d::Array<DownloadContent>> DownloadApi::List()
	{
		RestRequestor reqestor;
		auto status = co_await reqestor.postAsync(Api::DownloadList);
		if (status != RestRequestor::Status::Success) {
			co_return{};
		}
		auto contents = reqestor.getResponse().arrayView();
		s3d::Array<DownloadContent> out;
		out.clear();
		out.reserve(reqestor.getResponse().size());
		for (const auto& content : contents) {
			out.emplace_back(content);
		}
		co_return std::move(out);
	}
}
