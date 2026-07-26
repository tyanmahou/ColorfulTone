#pragma once
#include <core/Data/Genre/CTCF/AST/IStatement.hpp>
#include <core/Data/Genre/CTCF/AST/Statement/ExprStatement.hpp>
#include <memory>

namespace ct::ctcf::AST
{
	struct Root : INode
	{
		std::shared_ptr<ExprStatement> filter;
		std::shared_ptr<ExprStatement> selector;
	};
}